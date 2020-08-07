#include "SceneLine.h"

CSceneLine::CSceneLine(ISceneGraph *pSceneGraph):
    QtDrawShape<ILine>(pSceneGraph)
{
}

/// 添加点
void CSceneLine::AddPoint(const ScenePos &)
{
}

/// 移除指定位置点
void CSceneLine::RemovePoint(int)
{
}

/// 更新指定位置的
void CSceneLine::UpdatePoint(int, const ScenePos &)
{
}

///初始化多个位置点
void CSceneLine::SetMultPos(const vector<ScenePos> &)
{
}

/// 更新地图
void CSceneLine::UpdateMapNode(osgEarth::MapNode *pMapNode)
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

/// 初始化场景节点
void CSceneLine::InitSceneNode()
{
    QtDrawShape<ILine>::InitSceneNode();

    osgEarth::Feature* pFeature = new osgEarth::Feature(
                new osgEarth::LineString,
                osgEarth::SpatialReference::get("wgs84"), m_styleNode);

    pFeature->geoInterp() = osgEarth::GEOINTERP_GREAT_CIRCLE;

    m_pFeatureNode = new osgEarth::FeatureNode(pFeature);
    m_pFeatureNode->setDynamic(true);
    m_pOsgNode->addChild(m_pFeatureNode);
}

/// 初始化样式
void CSceneLine::InitStyle()
{
    m_styleNode.getOrCreate<osgEarth::LineSymbol>()
            ->stroke()->color() = osgEarth::Color(m_stColor.fR,m_stColor.fG
                                                  ,m_stColor.fB,m_stColor.fA);
    m_styleNode.getOrCreate<osgEarth::LineSymbol>()
            ->stroke()->width() = m_fLineWidth;

    m_styleNode.getOrCreate<osgEarth::LineSymbol>()
            ->stroke()->stipplePattern() = DOTTED_LINE == m_emType ? 0xF0F0 : 0xFFFF;

    m_styleNode.getOrCreate<osgEarth::AltitudeSymbol>()
            ->clamping() = osgEarth::AltitudeSymbol::CLAMP_RELATIVE_TO_TERRAIN;
    m_styleNode.getOrCreate<osgEarth::AltitudeSymbol>()
            ->technique() = osgEarth::AltitudeSymbol::TECHNIQUE_SCENE;
    m_styleNode.getOrCreate<osgEarth::AltitudeSymbol>()->binding()
            = osgEarth::AltitudeSymbol::BINDING_VERTEX;
    m_styleNode.getOrCreate<osgEarth::AltitudeSymbol>()
            ->verticalOffset() = 0.1;
}

/// 修改线宽
void CSceneLine::SetLineWidth(float fLineWidth)
{
    if(fLineWidth > 0 && fabs(m_fLineWidth-fLineWidth) > 1e-7)
    {
        m_fLineWidth = fLineWidth;
        m_pFeatureNode->getFeature()->style()->getOrCreate<osgEarth::LineSymbol>()
                ->stroke()->width() = m_fLineWidth;
    }
}

/// 设置线型
void CSceneLine::SetLineType(SCENE_LINE_TYPE lineType)
{
    if(m_emType != lineType)
    {
        m_emType = lineType;
        m_pFeatureNode->getFeature()->style()->getOrCreate<osgEarth::LineSymbol>()
                ->stroke()->stipplePattern() = DOTTED_LINE == m_emType ? 0xF0F0 : 0xFFFF;
    }
}

/// 修改颜色
void CSceneLine::SetColor(const SceneColor &rColor)
{
    if(m_stColor != rColor)
    {
        m_stColor = rColor;
        m_pFeatureNode->getFeature()->style()->getOrCreate<osgEarth::LineSymbol>()->stroke()->color()
                = osgEarth::Color(m_stColor.fR,m_stColor.fG,m_stColor.fB,m_stColor.fA);
    }
}
