#include "ScenePoint.h"

CScenePoint::CScenePoint(ISceneGraph *pSceneGraph)
    :QtDrawShape<IPoint>(pSceneGraph)
{
}

/// 更新地球
void CScenePoint::UpdateMapNode(osgEarth::MapNode *pMapNode)
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
void CScenePoint::InitSceneNode()
{
    QtDrawShape<IPoint>::InitSceneNode();

    osgEarth::Feature* pFeature = new osgEarth::Feature(
                new osgEarth::PointSet,
                osgEarth::SpatialReference::get("wgs84"), m_styleNode);

    m_pFeatureNode = new osgEarth::FeatureNode(pFeature);
    m_pFeatureNode->setDynamic(true);

    m_pOsgNode->addChild(m_pFeatureNode);
}

/// 更改位置
void CScenePoint::PosChanged()
{
    osgEarth::Geometry* pGeometry = m_pFeatureNode->getFeature()->getGeometry();
    if(pGeometry->size() < 1)
    {
        pGeometry->push_back(osg::Vec3d(m_unScenePos.fLon,m_unScenePos.fLat,m_unScenePos.fHeight));
    }
    else
    {
        pGeometry->at(0) = osg::Vec3d(m_unScenePos.fLon,m_unScenePos.fLat,m_unScenePos.fHeight);
    }

    /// 重新构建futureNode
    m_pFeatureNode->dirty();
}

/// 初始化样式
void CScenePoint::InitStyle()
{
    m_styleNode.getOrCreate<osgEarth::PointSymbol>()->fill()->color() = osgEarth::Color(m_stColor.fR,m_stColor.fG
                                                                                        ,m_stColor.fB,m_stColor.fA);
    m_styleNode.getOrCreate<osgEarth::PointSymbol>()->size() = m_fPointSize;
    m_styleNode.getOrCreate<osgEarth::PointSymbol>()->smooth() = true;

    m_styleNode.getOrCreate<osgEarth::AltitudeSymbol>()
            ->clamping()=osgEarth::AltitudeSymbol::CLAMP_RELATIVE_TO_TERRAIN;
    m_styleNode.getOrCreate<osgEarth::AltitudeSymbol>()->binding()
            = osgEarth::AltitudeSymbol::BINDING_VERTEX;
}

/// 设置点的大小
void CScenePoint::SetPointSize(float fPointSize)
{
    if(fPointSize > 0 && fabs(m_fPointSize - fPointSize) > 1e-7)
    {
        m_fPointSize = fPointSize;

        m_pFeatureNode->getFeature()->style()->getOrCreate<osgEarth::PointSymbol>()
                ->size() = m_fPointSize;
    }
}

/// 修改颜色
void CScenePoint::SetColor(const SceneColor & rColor)
{
    if(m_stColor != rColor)
    {
        m_stColor = rColor;
        m_pFeatureNode->getFeature()->style()->getOrCreate<osgEarth::PointSymbol>()->fill()->color()
                = osgEarth::Color(m_stColor.fR,m_stColor.fG,m_stColor.fB,m_stColor.fA);
    }
}
