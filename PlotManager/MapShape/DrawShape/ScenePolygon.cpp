#include <osg/Callback>
#include "ScenePolygon.h"

/// 多边形回调
class PolygonCallBack:public osg::Callback
{
public:
    PolygonCallBack(CScenePolygon* pPolygon):m_pPolygon(pPolygon)
    {
    }

    /// 回调函数
    bool run(osg::Object* object, osg::Object* data)
    {
        switch(m_pPolygon->m_emChangeType)
        {
        case UPDATE_POINT:
        {
            osgEarth::Geometry* pGeometry = m_pPolygon->m_pFeatureNode->getFeature()->getGeometry();
            pGeometry->at(m_pPolygon->m_nIndex).set(m_pPolygon->m_stScenePos.fLon,
                                                 m_pPolygon->m_stScenePos.fLat,
                                                 m_pPolygon->m_stScenePos.fHeight);
            /// 重新构建futureNode
            m_pPolygon->m_pFeatureNode->dirty();
            m_pPolygon->m_emChangeType=NO_CHANGE;
        }
            break;
        case REPLACE_POINS:
        {
            osgEarth::Geometry* pGeometry = m_pPolygon->m_pFeatureNode->getFeature()->getGeometry();
            pGeometry->clear();
            pGeometry->reserve(m_pPolygon->m_listAllPos.size());
            for(auto one : m_pPolygon->m_listAllPos)
            {
                pGeometry->push_back(osg::Vec3(one.fLon,
                                               one.fLat,
                                               one.fHeight));
            }
            m_pPolygon->m_pFeatureNode->dirty();
            m_pPolygon->m_emChangeType=NO_CHANGE;
        }
            break;
        }

        return traverse(object, data);
    }
private:
    CScenePolygon* m_pPolygon;
};

CScenePolygon::CScenePolygon(ISceneGraph *pSceneGraph):
    QtDrawShape<IPolygon>(pSceneGraph)
{
}

/// 添加位置点
void CScenePolygon::AddPoint(int nIndex, const ScenePos &rScenePos)
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
bool CScenePolygon::RemovePoint(int nIndex)
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

/// 更新指定位置点
bool CScenePolygon::UpdatePoint(int nIndex, const ScenePos &rPos)
{
    if(nIndex<0 || nIndex >= m_listAllPos.size())
    {
        return(false);
    }

    m_nIndex = nIndex;
    if(m_stScenePos != rPos)
    {
        m_stScenePos = rPos;
        m_emChangeType = UPDATE_POINT;
    }

    return(true);
}

/// 设置多个点
void CScenePolygon::SetMultPos(const vector<ScenePos> &vAllPoints)
{
    m_listAllPos.clear();

    for(auto one : vAllPoints)
    {
        m_listAllPos.push_back(one);
    }
    m_emChangeType = REPLACE_POINS;
}

/// 获取多个点位置
vector<ScenePos> CScenePolygon::GetMulPos()
{
    vector<ScenePos> vTempPos;
    vTempPos.reserve(m_listAllPos.size());
    for(auto one : m_listAllPos)
    {
        vTempPos.push_back(one);
    }

    return(vTempPos);
}

/// 设置填充颜色
void CScenePolygon::SetColor(const SceneColor &rColor)
{
    if(m_stColor != rColor)
    {
        m_stColor = rColor;
        m_pFeatureNode->getFeature()->style()->getOrCreate<osgEarth::PolygonSymbol>()
                ->fill()->color() = osgEarth::Color(m_stColor.fR,m_stColor.fG,
                                                    m_stColor.fB,m_stColor.fA);
    }
}

/// 获取颜色
const SceneColor &CScenePolygon::GetColor()
{
    return(m_stColor);
}

void CScenePolygon::UpdateMapNode(osgEarth::MapNode *pMapNode)
{
    m_pFeatureNode->setMapNode(pMapNode);
    if(pMapNode->isGeocentric())
    {
        m_pFeatureNode->setHorizonCulling(true);
    }
}

/// 初始化节点
void CScenePolygon::InitSceneNode()
{
    QtDrawShape<IPolygon>::InitSceneNode();

    osgEarth::Feature* pFeature = new osgEarth::Feature(
                new osgEarth::Polygon,
                osgEarth::SpatialReference::get("wgs84"), m_styleNode);

    m_pFeatureNode = new osgEarth::FeatureNode(pFeature);
    m_pFeatureNode->setDynamic(true);
    m_pPolygonCallBack = new PolygonCallBack(this);
    m_pFeatureNode->addUpdateCallback(m_pPolygonCallBack);
    m_pOsgNode->addChild(m_pFeatureNode);
}

void CScenePolygon::InitStyle()
{
    /// 多边形颜色
    m_styleNode.getOrCreate<osgEarth::PolygonSymbol>()
            ->fill()->color() = osgEarth::Color(m_stColor.fR,m_stColor.fG
                                                ,m_stColor.fB,m_stColor.fA);

    m_styleNode.getOrCreate<osgEarth::AltitudeSymbol>()
            ->clamping() = osgEarth::AltitudeSymbol::CLAMP_TO_TERRAIN;
    m_styleNode.getOrCreate<osgEarth::AltitudeSymbol>()
            ->technique() = osgEarth::AltitudeSymbol::TECHNIQUE_DRAPE;
    m_styleNode.getOrCreate<osgEarth::AltitudeSymbol>()
            ->verticalOffset() = 0.1;
}
