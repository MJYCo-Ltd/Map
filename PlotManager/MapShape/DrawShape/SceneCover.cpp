#include <osg/ShadeModel>
#include <osgEarth/PlaceNode>
#include <ISceneCore.h>
#include <Inner/ILoadResource.h>
#include "SceneCover.h"
#include "../Grid2TriangleStrip.h"

CSceneCover::CSceneCover(ISceneGraph *pSceneGraph):
    QtDrawShape<ICover>(pSceneGraph)
{
}

void CSceneCover::UpdateMapNode(osgEarth::MapNode *pMapNode)
{
    m_pDrapeableNode->setMapNode(pMapNode);
    m_pImage->setMapNode(pMapNode);
}

void CSceneCover::InitSceneNode()
{
    QtDrawShape<ICover>::InitSceneNode();
    m_pDrapeableNode = new osgEarth::DrapeableNode;
    m_pOsgNode->addChild(m_pDrapeableNode);
    m_pImage= new osgEarth::ImageOverlay(osgEarth::Config(),0);
    m_pImage->setDynamic(true);
    m_pOsgNode->addChild(m_pImage);
}

void CSceneCover::PosChanged()
{
//    string sImagePath = "ico/placemark32.png";
//    auto pImage = m_pSceneGraph->ResouceLoader()->LoadImage(sImagePath);
//    auto pPlaceNode = new osgEarth::PlaceNode(osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"),m_stScenePos.fLon,m_stScenePos.fLat,10),
//                                              "center",m_styleNode,pImage);
//    m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CModifyNode(m_pOsgNode,pPlaceNode,true));
}
#include <QDebug>
#include <GisMath/GisMath.h>
#include <Sofa/sofam.h>
/// 设置覆盖信息
void CSceneCover::SetCoverInfo(const CoverInfo & rstCoverInfo)
{
    return;
    CGrid2TriangleStrip m_Grid2TriangleStrip(rstCoverInfo.nXNum,rstCoverInfo.nYNum);

    /// 顶点数组
    osg::ref_ptr<osg::Vec3Array> pArray = new osg::Vec3Array;
    pArray->resize(rstCoverInfo.vPosInfo.size());

    /// 构建颜色数组
    osg::ref_ptr<osg::Vec4dArray> pColorArray = new osg::Vec4dArray;
    pColorArray->resize(rstCoverInfo.vPosInfo.size());


    double dStep = 25,dX;
    double dLong = 2*dStep;
    double dMin = rstCoverInfo.dMax - dLong;


    /// 计算地球节点位置

    const osgEarth::SpatialReference* pSpatial = osgEarth::SpatialReference::get("wgs84");
//    osgEarth::GeoPoint geoPoint(pSpatial,m_stScenePos.fLon,m_stScenePos.fLat,m_stScenePos.fHeight);

    double dLonRad=m_stScenePos.fLon *DD2R;
    double dLatRad=m_stScenePos.fLat *DD2R;
    osg::Vec3d vIn(120,24.0,0);
    osg::Vec3d vOut(vIn);

    double dLonCal,dLatCal;
    double dLonResult,dLatResult;

//    osg::Matrixd local2World;
//    geoPoint.createLocalToWorld(local2World);
    /// 设置
    for(int nIndex = 0; nIndex < rstCoverInfo.vPosInfo.size(); ++nIndex)
    {
//        vIn.x() = rstCoverInfo.vPosInfo[nIndex].dX;
//        vIn.y() = rstCoverInfo.vPosInfo[nIndex].dY;

//        vOut = vIn * local2World;
        GisMath::CalBaiser(dLonRad,dLatRad,0,rstCoverInfo.vPosInfo[nIndex].dY,dLonCal,dLatCal);
        GisMath::CalBaiser(dLonCal,dLatCal,DPI/2.,rstCoverInfo.vPosInfo[nIndex].dX,dLonResult,dLatResult);
        vIn.x() = dLonResult * DR2D;
        vIn.y() = dLatResult * DR2D;
        qDebug()<<nIndex<<'\t'<<rstCoverInfo.vPosInfo[nIndex].dX<<'\t'<<rstCoverInfo.vPosInfo[nIndex].dY;

        pSpatial->transformToWorld(vIn,vOut);

        pArray->at(nIndex) = vOut;

        if(rstCoverInfo.dMax - dLong < rstCoverInfo.vPosInfo[nIndex].dValue)
        {
            dX = (rstCoverInfo.vPosInfo[nIndex].dValue - dMin) / dStep;
            pColorArray->at(nIndex).r() = std::min(1.0,dX);
            pColorArray->at(nIndex).b() = std::max(0.,std::min(1.0,2.0-dX));
            pColorArray->at(nIndex).a() = 0.3;
        }
        else
        {
            pColorArray->at(nIndex).r() = 0.0;
            pColorArray->at(nIndex).b() = 1.0;
            pColorArray->at(nIndex).a() = 0.3;
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
    m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CModifyNode(m_pDrapeableNode,m_pCoverage,true));
}

/// 设置测点信息
void CSceneCover::SetCeDian(const vector<CeDian> & vAllCedian)
{

    string sImagePath = "ico/test.png";
    auto pImage = m_pSceneGraph->ResouceLoader()->LoadImage(sImagePath);


    /// 遍历所有的测点信息
    for(auto one : vAllCedian)
    {
        auto pPlaceNode = new osgEarth::PlaceNode(osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"),one.dLon,one.dLat,10),
                                                  one.sName,m_styleNode,pImage);
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CModifyNode(m_pOsgNode,pPlaceNode,true));
    }
}

/// 初始化样式
void CSceneCover::InitStyle()
{
    m_styleNode.getOrCreateSymbol<osgEarth::TextSymbol>()->encoding() = osgEarth::TextSymbol::ENCODING_UTF8;
    m_styleNode.getOrCreateSymbol<osgEarth::TextSymbol>()->alignment() = osgEarth::TextSymbol::ALIGN_LEFT_CENTER;
    m_styleNode.getOrCreateSymbol<osgEarth::TextSymbol>()->size() = 20;
    string sFontPath = GetExePath();
    sFontPath += "fonts/msyh.ttf";
    m_styleNode.getOrCreateSymbol<osgEarth::TextSymbol>()->font() = sFontPath;
    m_styleNode.getOrCreateSymbol<osgEarth::TextSymbol>()->fill()->color() = osgEarth::Color::White;
    m_styleNode.getOrCreateSymbol<osgEarth::TextSymbol>()->halo()->color() = osgEarth::Color::Black;
    m_styleNode.getOrCreate<osgEarth::AltitudeSymbol>()->clamping() =
            osgEarth::AltitudeSymbol::CLAMP_RELATIVE_TO_TERRAIN;
    m_styleNode.getOrCreate<osgEarth::IconSymbol>()->declutter() = false;
    m_styleNode.getOrCreate<osgEarth::IconSymbol>()->alignment() = osgEarth::IconSymbol::ALIGN_RIGHT_CENTER;
}

/// 设置最小值颜色
void CSceneCover::SetMinValueColor(const SceneColor &rMinColor)
{
    m_stMinColor = rMinColor;
}

/// 设置最大值颜色
void CSceneCover::SetMaxValueColor(const SceneColor &rMaxColor)
{
    m_stMaxColor = rMaxColor;
}
#include <osgEarth/ImageOverlay>
void CSceneCover::SetBound(const ScenePos &rLeftUp, const ScenePos &rRightDown, const string &sImagePath)
{
    m_pImage->setWest(rLeftUp.fLon);
    m_pImage->setEast(rRightDown.fLon);
    m_pImage->setNorth(rLeftUp.fLat);
    m_pImage->setSouth(rRightDown.fLat);

    auto pImage =  m_pSceneGraph->ResouceLoader()->LoadImage(sImagePath,0,0,false);
    m_pImage->setImage(pImage);
    m_pImage->setAlpha(0.5f);
    m_pImage->setDraped(true);
}
