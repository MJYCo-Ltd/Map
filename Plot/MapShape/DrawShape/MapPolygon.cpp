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

    m_bPosChanged=true;
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

    m_bPosChanged=true;
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
        m_bPosChanged=true;
        NodeChanged();
    }

    return(true);
}

/// 设置多个点
void CMapPolygon::SetMultPos(const std::vector<MapGeoPos> &vAllPoints)
{
    m_listAllPos.clear();

    for(auto one : vAllPoints)
    {
        m_listAllPos.push_back(one);
    }

    m_bPosChanged=true;
    NodeChanged();
}

/// 多边形颜色修改
void CMapPolygon::PolygonColorChanged()
{
    m_pNodeStyle->getOrCreate<osgEarth::PolygonSymbol>()
            ->fill()->color().set(m_stPolygonColor.fR,m_stPolygonColor.fG
                                  ,m_stPolygonColor.fB,m_stPolygonColor.fA);
}

/// 更新回调
void CMapPolygon::UpdateNode()
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
        m_pFeatureNode->dirty();
        m_bPosChanged=false;
    }

    ImplMapShape<IMapPolygon>::UpdateNode();
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
    ImplMapShape<IMapPolygon>::InitNode();

    osgEarth::Feature* pFeature = new osgEarth::Feature(
                new osgEarth::Polygon,
                osgEarth::SpatialReference::get("wgs84"));

    m_pNodeStyle = pFeature->style().operator->();
    InitStyle();
    m_pFeatureNode = new osgEarth::FeatureNode(pFeature);
    m_pFeatureNode->setDynamic(true);

    SetOsgNode(m_pFeatureNode.get());
}

/// 初始化样式
void CMapPolygon::InitStyle()
{
    /// 多边形颜色
    m_pNodeStyle->getOrCreate<osgEarth::PolygonSymbol>()
            ->fill()->color().set(m_stPolygonColor.fR,m_stPolygonColor.fG
                                  ,m_stPolygonColor.fB,m_stPolygonColor.fA);

    ImplMapShape<IMapPolygon>::InitStyle(m_pNodeStyle);
}
