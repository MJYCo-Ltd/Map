#include <osg/ShadeModel>
#include <osgEarth/PlaceNode>
#include <ISceneCore.h>
#include <Inner/ILoadResource.h>
#include <Inner/OsgExtern/Grid2TriangleStrip.h>
#include "MapCoverImage.h"


void CMapCoverImage::UpdateMapNode(osgEarth::MapNode *pMapNode)
{
    m_pDrapeableNode->setMapNode(pMapNode);

    if(m_pImage.valid()) m_pImage->setMapNode(pMapNode);
}

void CMapCoverImage::InitNode()
{
    ImplMapSceneNode<IMapCoverImage>::InitNode();
    InitStyle();

    m_pCoverRoot = new osg::Group;
    m_pDrapeableNode = new osgEarth::DrapeableNode;
    m_pCoverRoot->addChild(m_pDrapeableNode);
    SetOsgNode(m_pCoverRoot.get());
}

/// 设置覆盖信息
void CMapCoverImage::SetCoverInfo(const CoverInfo & rstCoverInfo)
{
    if(m_pCoverage.valid())
    {
        /// 去掉之前的数据
        DelNode(m_pDrapeableNode,m_pCoverage);
    }

    CGrid2TriangleStrip m_Grid2TriangleStrip(rstCoverInfo.nXNum,rstCoverInfo.nYNum);

    /// 顶点数组
    osg::ref_ptr<osg::Vec3Array> pArray = new osg::Vec3Array;
    pArray->resize(rstCoverInfo.vPosInfo.size());

    /// 构建颜色数组
    osg::ref_ptr<osg::Vec4dArray> pColorArray = new osg::Vec4dArray;
    pColorArray->resize(rstCoverInfo.vPosInfo.size());


    double dValue,dX;
    double dStep1(1.0),dStep2(2.0),dStep3(3.0);


    /// 计算地球节点位置

    const osgEarth::SpatialReference* pSpatial = osgEarth::SpatialReference::get("wgs84");

    osg::Vec3d vIn(120,24.0,0);
    osg::Vec3d vOut(vIn);

    /// 设置
    for(int nIndex = 0; nIndex < rstCoverInfo.vPosInfo.size(); ++nIndex)
    {
        vIn.x() = rstCoverInfo.vPosInfo[nIndex].dX;
        vIn.y() = rstCoverInfo.vPosInfo[nIndex].dY;

        if (vIn.x() < 107 || vIn.x() > 122 || vIn.y()<24 || vIn.y() > 30.5) pColorArray->at(nIndex).a()=0.;
        else pColorArray->at(nIndex).a() = 0.9;

        pSpatial->transformToWorld(vIn,vOut);

        pArray->at(nIndex) = vOut;

        dValue = rstCoverInfo.vPosInfo[nIndex].dValue;
        if(dValue <0 ) dValue = 0;
        else if(dValue > 4) dValue = 4;


        if(dStep1 > dValue)
        {
            dX = dValue;
            pColorArray->at(nIndex).r() = dX*0.4549  + 0.5215686;
            pColorArray->at(nIndex).g() = dX*-0.18429+ 0.2039;
            pColorArray->at(nIndex).b() = dX*-0.2823 + 0.3333;
        }
        else if(dStep2 > dValue)
        {
            dX = (dValue - dStep1);
            pColorArray->at(nIndex).r() = dX*-0.427471 + 0.976471;
            pColorArray->at(nIndex).g() = dX*0.96469   + 0.01961;
            pColorArray->at(nIndex).b() = dX*0.42353   + 0.05098;
        }
        else if(dStep3 > dValue)
        {
            dX = (dValue - dStep2);
            pColorArray->at(nIndex).r() = dX*-0.52155 + 0.549;
            pColorArray->at(nIndex).g() = dX*-0.49019 + 0.9843;
            pColorArray->at(nIndex).b() = dX*0.52549  + 0.47451;
        }
        else
        {
            dX = (dValue - dStep3);
            pColorArray->at(nIndex).r() = dX*0.141176 + 0.02745;
            pColorArray->at(nIndex).g() = dX*-0.42745 + 0.49411;
            pColorArray->at(nIndex).b() = dX*-0.33333 + 1.;
        }

    }

    m_pCoverage = new osg::Geometry;
    osg::ShadeModel* pShadeModel = new osg::ShadeModel;
    m_pCoverage->getOrCreateStateSet()->setAttribute(pShadeModel,osg::StateAttribute::ON);
    m_pCoverage->getOrCreateStateSet()->setMode(GL_BLEND,osg::StateAttribute::ON);

    /// 设置定点数组
    m_pCoverage->setVertexArray(pArray.get());
    /// 设置颜色数组
    m_pCoverage->setColorArray(pColorArray,osg::Array::BIND_PER_VERTEX);

    const osg::MixinVector<osg::VectorGLuint>& rIndex = m_Grid2TriangleStrip.GetIndex();
    /// 添加绘制体
    for(int nIndex=0; nIndex < rIndex.size(); ++nIndex)
    {
        m_pCoverage->addPrimitiveSet(new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLE_STRIP,rIndex[nIndex].size(),&rIndex[nIndex].front()));
    }

    /// 增加显示
    AddNode(m_pDrapeableNode,m_pCoverage);
}

/// 设置测点信息
void CMapCoverImage::SetCeDian(const std::vector<CeDian> & vAllCedian)
{

    std::string sImagePath = "ico/test.png";
    auto pImage = m_pSceneGraph->ResouceLoader()->LoadImage(sImagePath);


    /// 遍历所有的测点信息
    for(auto one : vAllCedian)
    {
        auto pPlaceNode = new osgEarth::PlaceNode(osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"),one.dLon,one.dLat,10),
                                                  one.sName,osgEarth::Style(),pImage);
        AddNode(m_pCoverRoot.get(),pPlaceNode);
    }
}

/// 初始化样式
void CMapCoverImage::InitStyle()
{
    m_pNodeStyle = new osgEarth::Style;
    ImplMapSceneNode<IMapCoverImage>::InitStyle(m_pNodeStyle);

    m_pNodeStyle->getOrCreateSymbol<osgEarth::TextSymbol>()->encoding() = osgEarth::TextSymbol::ENCODING_UTF8;
    m_pNodeStyle->getOrCreateSymbol<osgEarth::TextSymbol>()->alignment() = osgEarth::TextSymbol::ALIGN_LEFT_CENTER;
    m_pNodeStyle->getOrCreateSymbol<osgEarth::TextSymbol>()->size() = 20;
    std::string sFontPath = GetDataPath();
    sFontPath += "fonts/msyh.ttf";
    m_pNodeStyle->getOrCreateSymbol<osgEarth::TextSymbol>()->font() = sFontPath;
    m_pNodeStyle->getOrCreateSymbol<osgEarth::TextSymbol>()->fill()->color() = osgEarth::Color::White;
    m_pNodeStyle->getOrCreateSymbol<osgEarth::TextSymbol>()->halo()->color() = osgEarth::Color::Black;
    m_pNodeStyle->getOrCreate<osgEarth::IconSymbol>()->declutter() = false;
    m_pNodeStyle->getOrCreate<osgEarth::IconSymbol>()->alignment() = osgEarth::IconSymbol::ALIGN_RIGHT_CENTER;
}

/// 设置最小值颜色
void CMapCoverImage::SetMinValueColor(const SceneColor &rMinColor)
{
    m_stMinColor = rMinColor;
}

/// 设置最大值颜色
void CMapCoverImage::SetMaxValueColor(const SceneColor &rMaxColor)
{
    m_stMaxColor = rMaxColor;
}
#include <osgEarth/ImageOverlay>
void CMapCoverImage::SetBound(const MapGeoPos &rLeftUp, const MapGeoPos &rRightDown, const std::string &sImagePath)
{
    if(!m_pImage.valid())
    {
        m_pImage= new osgEarth::ImageOverlay(osgEarth::Config(),0);
        m_pImage->setDynamic(true);
        AddNode(m_pCoverRoot.get(),m_pImage);
    }

    m_pImage->setWest(rLeftUp.fLon);
    m_pImage->setEast(rRightDown.fLon);
    m_pImage->setNorth(rLeftUp.fLat);
    m_pImage->setSouth(rRightDown.fLat);

    auto pImage =  m_pSceneGraph->ResouceLoader()->LoadImage(sImagePath,0,0,false);
    m_pImage->setImage(pImage);
    m_pImage->setAlpha(0.5f);
    m_pImage->setDraped(true);
}
