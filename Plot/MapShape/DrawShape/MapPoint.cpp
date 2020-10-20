#include "MapPoint.h"

/// 更新地球
void CMapPoint::UpdateMapNode(osgEarth::MapNode *pMapNode)
{
    m_pFeatureNode->setMapNode(pMapNode);
    if(pMapNode->isGeocentric())
    {
        m_pFeatureNode->getFeature()->style()->getOrCreate<osgEarth::RenderSymbol>()->depthTest()=true;
        m_pFeatureNode->setHorizonCulling(true);
    }
    else
    {
        m_pFeatureNode->getFeature()->style()->getOrCreate<osgEarth::RenderSymbol>()->depthTest()=false;
    }
}

/// 初始化节点
void CMapPoint::InitNode()
{
    ImplMapSceneNode<IMapPoint>::InitNode();

    osgEarth::Feature* pFeature = new osgEarth::Feature(
                new osgEarth::PointSet,
                osgEarth::SpatialReference::get("wgs84"));

    m_pNodeStyle = pFeature->style().operator->();

    m_pFeatureNode = new osgEarth::FeatureNode(pFeature);
    m_pFeatureNode->setDynamic(true);

    SetOsgNode(m_pFeatureNode.get());
    InitStyle();
}

/// 更改位置
void CMapPoint::PosChanged()
{
    osgEarth::Geometry* pGeometry = m_pFeatureNode->getFeature()->getGeometry();
    if(pGeometry->size() < 1)
    {
        pGeometry->push_back(osg::Vec3(m_stGeoPos.fLon,
                                        m_stGeoPos.fLat,
                                        m_stGeoPos.fHeight));
    }
    else
    {
        pGeometry->at(0) = osg::Vec3(m_stGeoPos.fLon,
                                      m_stGeoPos.fLat,
                                      m_stGeoPos.fHeight);
    }

    /// 重新构建futureNode
    m_pFeatureNode->dirty();
    NodeChanged();
}

/// 初始化样式
void CMapPoint::InitStyle()
{
    m_pNodeStyle->getOrCreate<osgEarth::PointSymbol>()->fill()->color() = osgEarth::Color(m_stColor.fR,m_stColor.fG
                                                                                        ,m_stColor.fB,m_stColor.fA);
    m_pNodeStyle->getOrCreate<osgEarth::PointSymbol>()->size() = m_fPointSize;
    m_pNodeStyle->getOrCreate<osgEarth::PointSymbol>()->smooth() = true;

    m_pNodeStyle->getOrCreate<osgEarth::AltitudeSymbol>()->binding()
            = osgEarth::AltitudeSymbol::BINDING_VERTEX;
    ImplMapSceneNode<IMapPoint>::InitStyle();
}

/// 修改
void CMapPoint::UpdateNode()
{
    ImplMapSceneNode<IMapPoint>::UpdateNode();
}

/// 点大小修改
void CMapPoint::PointSizeChanged()
{
    m_pNodeStyle->getOrCreate<osgEarth::PointSymbol>()->size() = m_fPointSize;
}

/// 颜色修改
void CMapPoint::ColorChanged()
{
    m_pNodeStyle->getOrCreate<osgEarth::PointSymbol>()->fill()->color() = osgEarth::Color(m_stColor.fR,m_stColor.fG
                                                                                        ,m_stColor.fB,m_stColor.fA);
}
