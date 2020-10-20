#include "SceneLine.h"

/// 添加点
void CSceneLine::AddPoint(int nIndex, const ScenePos &rScenePos)
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
bool CSceneLine::RemovePoint(int nIndex)
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
bool CSceneLine::UpdatePoint(int nIndex, const ScenePos &rPos)
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
void CSceneLine::SetMultPos(const vector<ScenePos> & vAllPoints)
{
    m_listAllPos.clear();

    for(auto one : vAllPoints)
    {
        m_listAllPos.push_back(one);
    }

    ShapeChanged();
}

std::vector<ScenePos> CSceneLine::GetMulPos() const
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

void CSceneLine::CreateShape()
{
    m_pDrawArrays = new osg::DrawArrays(GL_LINE_STRIP,0,m_pVertexArray->size());
    m_pGeometry->addPrimitiveSet(m_pDrawArrays);
}

void CSceneLine::UpdateShape()
{
    m_pVertexArray->resize(m_listAllPos.size());
    int nIndex=0;
    for(auto one : m_listAllPos)
    {
        m_pVertexArray->at(nIndex++).set(one.fX,one.fY,one.fZ);
    }

    m_pDrawArrays->setCount(m_pVertexArray->size());
}
