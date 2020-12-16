#include "MapPolygon.h"

/// 添加位置点
void CMapPolygon::AddPoint(int nIndex, const MapGeoPos &rScenePos)
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

    NodeChanged();
}

/// 移除指定位置点
bool CMapPolygon::RemovePoint(int nIndex)
{
    if(nIndex<0 || nIndex >= m_listAllPos.size())
    {
        return(false);
    }

    auto pIter = m_listAllPos.begin();
    for(int i=0; i<nIndex;++i,++pIter){}
    m_listAllPos.erase(pIter);

    NodeChanged();
    return(true);
}

/// 更新指定位置点
bool CMapPolygon::UpdatePoint(int nIndex, const MapGeoPos &rPos)
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
        NodeChanged();
    }

    return(true);
}

/// 设置多个点
void CMapPolygon::SetMultPos(const vector<MapGeoPos> &vAllPoints)
{
    m_listAllPos.clear();

    for(auto one : vAllPoints)
    {
        m_listAllPos.push_back(one);
    }
    NodeChanged();
}

/// 多边形颜色修改
void CMapPolygon::PolygonColorChanged()
{
    m_pNodeStyle->getOrCreate<osgEarth::PolygonSymbol>()
            ->fill()->color() = osgEarth::Color(m_stPolygonColor.fR,m_stPolygonColor.fG
                                                ,m_stPolygonColor.fB,m_stPolygonColor.fA);
}

/// 更新回调
void CMapPolygon::UpdateNode()
{
    osgEarth::Geometry* pGeometry = m_pFeatureNode->getFeature()->getGeometry();
    int nIndex(0);
    pGeometry->resize(m_listAllPos.size());
    for(auto one : m_listAllPos)
    {
        pGeometry->at(nIndex++).set(one.fLon,one.fLat,one.fHeight);
    }
    m_pFeatureNode->dirty();

    ImplMapSceneNode<IMapPolygon>::UpdateNode();
}

void CMapPolygon::UpdateMapNode(osgEarth::MapNode *pMapNode)
{
    m_pFeatureNode->setMapNode(pMapNode);
    if(pMapNode->isGeocentric())
    {
        m_pFeatureNode->setHorizonCulling(true);
    }
}

/// 初始化节点
void CMapPolygon::InitNode()
{
    ImplMapSceneNode<IMapPolygon>::InitNode();

    osgEarth::Feature* pFeature = new osgEarth::Feature(
                new osgEarth::Polygon,
                osgEarth::SpatialReference::get("wgs84"));

    m_pNodeStyle = pFeature->style().operator->();
    m_pFeatureNode = new osgEarth::FeatureNode(pFeature);
    m_pFeatureNode->setDynamic(true);

    SetOsgNode(m_pFeatureNode.get());
    InitStyle();
}

/// 初始化样式
void CMapPolygon::InitStyle()
{
    /// 多边形颜色
    m_pNodeStyle->getOrCreate<osgEarth::PolygonSymbol>()
            ->fill()->color() = osgEarth::Color(m_stPolygonColor.fR,m_stPolygonColor.fG
                                                ,m_stPolygonColor.fB,m_stPolygonColor.fA);

    m_pNodeStyle->getOrCreate<osgEarth::AltitudeSymbol>()
            ->technique() = osgEarth::AltitudeSymbol::TECHNIQUE_DRAPE;
    m_pNodeStyle->getOrCreate<osgEarth::AltitudeSymbol>()
            ->verticalOffset() = 0.1;
}
