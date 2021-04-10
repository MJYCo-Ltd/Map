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
    ImplMapShape<IMapPoint>::InitNode();

    osgEarth::Feature* pFeature = new osgEarth::Feature(
                new osgEarth::PointSet,
                osgEarth::SpatialReference::get("wgs84"));

    m_pNodeStyle = pFeature->style().operator->();

    InitStyle();

    m_pFeatureNode = new osgEarth::FeatureNode(pFeature);
    m_pFeatureNode->setDynamic(true);

    SetOsgNode(m_pFeatureNode.get());
}

/// 更改位置
void CMapPoint::PosChanged()
{
    m_bPosChanged=true;
    NodeChanged();
}

/// 初始化样式
void CMapPoint::InitStyle()
{
    m_pNodeStyle->getOrCreate<osgEarth::PointSymbol>()->fill()->color().set(m_stPointColor.fR,m_stPointColor.fG
                                                                            ,m_stPointColor.fB,m_stPointColor.fA);
    m_pNodeStyle->getOrCreate<osgEarth::PointSymbol>()->size() = m_fPointSize;
    m_pNodeStyle->getOrCreate<osgEarth::PointSymbol>()->smooth()=true;

    ImplMapShape<IMapPoint>::InitStyle(m_pNodeStyle);
}

/// 修改
void CMapPoint::UpdateNode()
{
    if(m_bPosChanged)
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
            pGeometry->at(0).set(m_stGeoPos.fLon,
                                 m_stGeoPos.fLat,
                                 m_stGeoPos.fHeight);
        }

        /// 重新构建futureNode
        m_pFeatureNode->dirty();
        m_bPosChanged=false;
    }
    ImplMapShape<IMapPoint>::UpdateNode();
}

/// 点大小修改
void CMapPoint::PointSizeChanged()
{
    m_pNodeStyle->getOrCreate<osgEarth::PointSymbol>()->size() = m_fPointSize;
}

/// 颜色修改
void CMapPoint::ColorChanged()
{
    m_pNodeStyle->getOrCreate<osgEarth::PointSymbol>()->fill()->color().set(m_stPointColor.fR,m_stPointColor.fG
                                                                            ,m_stPointColor.fB,m_stPointColor.fA);
}
