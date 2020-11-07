#include "ScenePolygon.h"

/// 添加点
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

    ShapeChanged();
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

    ShapeChanged();
    return(true);
}

/// 更新指定位置的
bool CScenePolygon::UpdatePoint(int nIndex, const ScenePos &rPos)
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
        ShapeChanged();
    }

    return(true);
}

///初始化多个位置点
void CScenePolygon::SetMultPos(const vector<ScenePos> & vAllPoints)
{
    m_listAllPos.clear();

    for(auto one : vAllPoints)
    {
        m_listAllPos.push_back(one);
    }

    ShapeChanged();
}

/// 获取多个位置点
std::vector<ScenePos> CScenePolygon::GetMulPos() const
{
    std::vector<ScenePos> vTempPos;
    vTempPos.resize(m_listAllPos.size());
    int nIndex(0);
    for(auto one : m_listAllPos)
    {
        ScenePos& rPos = vTempPos.at(nIndex++);

        rPos.fX = one.fX;
        rPos.fY = one.fY;
        rPos.fZ = one.fZ;
    }

    return(vTempPos);
}

/// 创建多边形
void CScenePolygon::CreateShape()
{
    m_pTess = new osgUtil::Tessellator;
    m_pTess->setTessellationType(osgUtil::Tessellator::TESS_TYPE_POLYGONS);
    m_pTess->setWindingType(osgUtil::Tessellator::TESS_WINDING_ODD);

    m_pDrawArrays = new osg::DrawArrays(GL_POLYGON,0,m_pVertexArray->size());
    m_pGeometry->addPrimitiveSet(m_pDrawArrays);
}

/// 更新多边形
void CScenePolygon::UpdateShape()
{
    m_pVertexArray->resize(m_listAllPos.size());
    int nIndex=0;
    for(auto one : m_listAllPos)
    {
        m_pVertexArray->at(nIndex++).set(one.fX,one.fY,one.fZ);
    }

    m_pDrawArrays->setCount(m_pVertexArray->size());

    /// 进行凹多边形裁剪
    m_pTess->retessellatePolygons(*m_pGeometry);
}
