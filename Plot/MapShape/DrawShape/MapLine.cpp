#include "MapLine.h"

/// 添加点
void CMapLine::AddPoint(int nIndex, const MapGeoPos &rScenePos)
{
    if(nIndex <=0)
    {
        m_listAllPos.push_front(rScenePos);
    }
    else if(nIndex >= m_listAllPos.size())
    {
        m_listAllPos.push_back(rScenePos);
    }
    else
    {
        auto pIter = m_listAllPos.begin();
        for(int i=0; i<nIndex;++i,++pIter){}

        m_listAllPos.insert(pIter,rScenePos);
    }

    m_bPosChanged=true;
    NodeChanged();
}

/// 移除指定位置点
bool CMapLine::RemovePoint(int nIndex)
{
    if(nIndex<0 || nIndex >= m_listAllPos.size())
    {
        return(false);
    }

    auto pIter = m_listAllPos.begin();
    for(int i=0; i<nIndex;++i,++pIter){}
    m_listAllPos.erase(pIter);

    m_bPosChanged=true;
    NodeChanged();
    return(true);
}

/// 更新指定位置的
bool CMapLine::UpdatePoint(int nIndex, const MapGeoPos &rPos)
{
    if(nIndex<0 || nIndex >= m_listAllPos.size())
    {
        return(false);
    }

    auto pIter = m_listAllPos.begin();
    for(int i=0; i<nIndex;++i,++pIter){}

    if(*pIter != rPos)
    {
        *pIter = rPos;
        m_bPosChanged=true;
        NodeChanged();
    }

    return(true);
}

///初始化多个位置点
void CMapLine::SetMultPos(const std::vector<MapGeoPos> & vAllPoints)
{
    m_listAllPos.clear();

    for(auto one : vAllPoints)
    {
        m_listAllPos.push_back(one);
    }

    m_bPosChanged=true;
    NodeChanged();
}

/// 获取位置点
std::vector<MapGeoPos> CMapLine::GetMulPos() const
{
    std::vector<MapGeoPos> vTempPos;
    vTempPos.resize(m_listAllPos.size());
    int nIndex(0);
    for(auto one : m_listAllPos)
    {
        MapGeoPos& rPos = vTempPos.at(nIndex++);

        rPos.fLon = one.fLon;
        rPos.fLat = one.fLat;
        rPos.fHeight = one.fHeight;
    }

    return(vTempPos);
}

///更新节点
void CMapLine::UpdateNode()
{
    if(m_bPosChanged)
    {
        osgEarth::Geometry* pGeometry = m_pFeatureNode->getFeature()->getGeometry();
        int nIndex(0);
        pGeometry->resize(m_listAllPos.size());
        for(auto one : m_listAllPos)
        {
            pGeometry->at(nIndex++).set(one.fLon,one.fLat,one.fHeight);
        }
        m_bPosChanged=false;
    }

    ///此处dirty会重新加载style
    m_pFeatureNode->dirty();
    ImplMapSceneNode<IMapLine>::UpdateNode();
}

/// 更新地图
void CMapLine::UpdateMapNode(osgEarth::MapNode *pMapNode)
{
    m_pFeatureNode->setMapNode(pMapNode);
    if(pMapNode->isGeocentric())
    {
        m_pNodeStyle->getOrCreate<osgEarth::RenderSymbol>()->depthTest()=true;
        m_pFeatureNode->setHorizonCulling(true);
    }
    else
    {
        m_pNodeStyle->getOrCreate<osgEarth::RenderSymbol>()->depthTest()=false;
    }
}

/// 初始化场景节点
void CMapLine::InitNode()
{
    ImplMapSceneNode<IMapLine>::InitNode();

    osgEarth::Feature* pFeature = new osgEarth::Feature(
                new osgEarth::LineString,
                osgEarth::SpatialReference::get("wgs84"));

    m_pNodeStyle = pFeature->style().operator->();

    InitStyle();
    m_pFeatureNode = new osgEarth::FeatureNode(pFeature);
    m_pFeatureNode->setDynamic(true);

    m_pFeatureNode->dirty();

    SetOsgNode(m_pFeatureNode.get());
}

/// 初始化样式
void CMapLine::InitStyle()
{
    m_pNodeStyle->getOrCreate<osgEarth::LineSymbol>()
            ->stroke()->color() = osgEarth::Color(m_stLineColor.fR,m_stLineColor.fG
                                                  ,m_stLineColor.fB,m_stLineColor.fA);
    m_pNodeStyle->getOrCreate<osgEarth::LineSymbol>()
            ->stroke()->width() = m_fLineWidth;
    m_pNodeStyle->getOrCreate<osgEarth::LineSymbol>()
            ->stroke()->widthUnits() = osgEarth::Units::KILOMETERS;

    m_pNodeStyle->getOrCreate<osgEarth::LineSymbol>()
            ->stroke()->stipplePattern() = MAP_DOTTED_LINE == m_emLineType ? 0xF0F0 : 0xFFFF;

    m_pNodeStyle->getOrCreate<osgEarth::LineSymbol>()->imageURI() = osgEarth::StringExpression("E:/Git/Bin/Images/road.png");

    m_pNodeStyle->getOrCreate<osgEarth::AltitudeSymbol>()
            ->technique() = osgEarth::AltitudeSymbol::TECHNIQUE_DRAPE;
    m_pNodeStyle->getOrCreate<osgEarth::AltitudeSymbol>()
            ->clamping() = osgEarth::AltitudeSymbol::CLAMP_RELATIVE_TO_TERRAIN;
//    m_pNodeStyle->getOrCreate<osgEarth::AltitudeSymbol>()->binding()
//            = osgEarth::AltitudeSymbol::BINDING_VERTEX;

    ImplMapSceneNode<IMapLine>::InitStyle(m_pNodeStyle);
}

/// 修改线宽
void CMapLine::LineWidthChanged()
{
    m_pNodeStyle->getOrCreate<osgEarth::LineSymbol>()
            ->stroke()->width() = m_fLineWidth;
}

/// 设置线型
void CMapLine::LineTypeChanged()
{
    m_pNodeStyle->getOrCreate<osgEarth::LineSymbol>()
            ->stroke()->stipplePattern() = MAP_DOTTED_LINE == m_emLineType ? 0xF0F0 : 0xFFFF;
}

/// 修改颜色
void CMapLine::LineColorChanged()
{
    m_pNodeStyle->getOrCreate<osgEarth::LineSymbol>()->stroke()->color()
            = osgEarth::Color(m_stLineColor.fR,m_stLineColor.fG,m_stLineColor.fB,m_stLineColor.fA);
}
