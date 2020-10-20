#include <osg/Callback>
#include "MapPoint.h"
class PointCallBack:public osg::Callback
{
public:
    PointCallBack(CMapPoint* pPoint):m_pPoint(pPoint)
    {
    }

    bool run(osg::Object* object, osg::Object* data)
    {
        if(m_pPoint->m_bUpdate)
        {
            osgEarth::Geometry* pGeometry = m_pPoint->m_pFeatureNode->getFeature()->getGeometry();
            if(pGeometry->size() < 1)
            {
                pGeometry->push_back(osg::Vec3(m_pPoint->m_stGeoPos.fLon,
                                                m_pPoint->m_stGeoPos.fLat,
                                                m_pPoint->m_stGeoPos.fHeight));
            }
            else
            {
                pGeometry->at(0) = osg::Vec3(m_pPoint->m_stGeoPos.fLon,
                                              m_pPoint->m_stGeoPos.fLat,
                                              m_pPoint->m_stGeoPos.fHeight);
            }

            /// 重新构建futureNode
            m_pPoint->m_pFeatureNode->dirty();
            m_pPoint->m_bUpdate=false;
        }

        return traverse(object, data);
    }
private:
    CMapPoint* m_pPoint;
};

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
    InitStyle();

    osgEarth::Feature* pFeature = new osgEarth::Feature(
                new osgEarth::PointSet,
                osgEarth::SpatialReference::get("wgs84"), m_styleNode);

    m_pFeatureNode = new osgEarth::FeatureNode(pFeature);
    m_pFeatureNode->setDynamic(true);

    m_pPointCallBack = new PointCallBack(this);
    m_pFeatureNode->setUpdateCallback(m_pPointCallBack.get());

    SetOsgNode(m_pFeatureNode.get());
}

/// 更改位置
void CMapPoint::PosChanged()
{
    m_bUpdate = true;
}

/// 初始化样式
void CMapPoint::InitStyle()
{
    m_styleNode.getOrCreate<osgEarth::PointSymbol>()->fill()->color() = osgEarth::Color(m_stColor.fR,m_stColor.fG
                                                                                        ,m_stColor.fB,m_stColor.fA);
    m_styleNode.getOrCreate<osgEarth::PointSymbol>()->size() = m_fPointSize;
    m_styleNode.getOrCreate<osgEarth::PointSymbol>()->smooth() = true;

    m_styleNode.getOrCreate<osgEarth::AltitudeSymbol>()->binding()
            = osgEarth::AltitudeSymbol::BINDING_VERTEX;
    ImplMapSceneNode<IMapPoint>::InitStyle();
}

void CMapPoint::PointSizeChanged()
{
    m_pFeatureNode->getFeature()->style()->getOrCreate<osgEarth::PointSymbol>()
            ->size() = m_fPointSize;
}

void CMapPoint::ColorChanged()
{
    m_pFeatureNode->getFeature()->style()->getOrCreate<osgEarth::PointSymbol>()->fill()->color()
            = osgEarth::Color(m_stColor.fR,m_stColor.fG,m_stColor.fB,m_stColor.fA);
}
