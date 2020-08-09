#include <osg/Callback>
#include "SceneLine.h"

class LineCallBack:public osg::Callback
{
public:
    LineCallBack(CSceneLine* pLine):m_pLine(pLine)
    {
    }

    bool run(osg::Object* object, osg::Object* data)
    {
        switch(m_pLine->m_emChangeType)
        {
        case UPDATE_POINT:
        {
            osgEarth::Geometry* pGeometry = m_pLine->m_pFeatureNode->getFeature()->getGeometry();
            pGeometry->at(m_pLine->m_nIndex).set(m_pLine->m_unScenePos.fLon,
                                                 m_pLine->m_unScenePos.fLat,
                                                 m_pLine->m_unScenePos.fHeight);
            /// 重新构建futureNode
            m_pLine->m_pFeatureNode->dirty();
            m_pLine->m_emChangeType=NO_CHANGE;
        }
            break;
        case REPLACE_POINS:
        {
            osgEarth::Geometry* pGeometry = m_pLine->m_pFeatureNode->getFeature()->getGeometry();
            pGeometry->clear();
            pGeometry->reserve(m_pLine->m_listAllPos.size());
            for(auto one : m_pLine->m_listAllPos)
            {
                pGeometry->push_back(osg::Vec3(one.fLon,
                                               one.fLat,
                                               one.fHeight));
            }
            m_pLine->m_pFeatureNode->dirty();
            m_pLine->m_emChangeType=NO_CHANGE;
        }
            break;
        }

        return traverse(object, data);
    }
private:
    CSceneLine* m_pLine;
};

CSceneLine::CSceneLine(ISceneGraph *pSceneGraph):
    QtDrawShape<ILine>(pSceneGraph)
{
}

/// 添加点
void CSceneLine::AddPoint(int nIndex, const ScenePos & rScenePos)
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

    m_emChangeType = REPLACE_POINS;
}

/// 移除指定位置点
bool CSceneLine::RemovePoint(int nIndex)
{
    if(nIndex<0 || nIndex >= m_listAllPos.size())
    {
        return(false);
    }

    auto pIter = m_listAllPos.begin();
    for(int i=0; i<nIndex;++i,++pIter){}
    m_listAllPos.erase(pIter);

    m_emChangeType = REPLACE_POINS;
    return(true);
}

/// 更新指定位置的
bool CSceneLine::UpdatePoint(int nIndex, const ScenePos &rPos)
{
    if(nIndex<0 || nIndex >= m_listAllPos.size())
    {
        return(false);
    }

    m_nIndex = nIndex;
    if(m_unScenePos != rPos)
    {
        m_unScenePos = rPos;
        m_emChangeType = UPDATE_POINT;
    }

    return(true);
}

///初始化多个位置点
void CSceneLine::SetMultPos(const vector<ScenePos> & vAllPoints)
{
    m_listAllPos.clear();

    for(auto one : vAllPoints)
    {
        m_listAllPos.push_back(one);
    }
    m_emChangeType = REPLACE_POINS;
}

vector<ScenePos> CSceneLine::GetMulPos()
{
    vector<ScenePos> vTempPos;
    vTempPos.reserve(m_listAllPos.size());
    for(auto one : m_listAllPos)
    {
        vTempPos.push_back(one);
    }

    return(vTempPos);
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

    m_pFeatureNode = new osgEarth::FeatureNode(pFeature);
    m_pFeatureNode->setDynamic(true);
    m_pLineCallBack = new LineCallBack(this);
    m_pFeatureNode->addUpdateCallback(m_pLineCallBack);
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
            ->technique() = osgEarth::AltitudeSymbol::TECHNIQUE_MAP;
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
