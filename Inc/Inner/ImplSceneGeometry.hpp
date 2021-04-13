#ifndef IMPL_SCENE_GEOMETRY_H
#define IMPL_SCENE_GEOMETRY_H

#include <Inner/ImplSceneShape.hpp>

/**
 *  实现ISceneGeomerty所有的接口
 */
template <typename T>
class ImplSceneGeometry:public ImplSceneShape<T>
{
public:
    CONSTRUCTOR(ImplSceneGeometry,ImplSceneShape<T>)

    /**
     * @brief增加一个点
     */
    void AddPoint(int nIndex, const ScenePos &rScenePos)
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

        NeedUpdate();
    }

    /**
     * @brief 移除一个点
     * @return
     */
    bool RemovePoint(int nIndex)
    {
        if(nIndex<0 || nIndex >= m_listAllPos.size())
        {
            return(false);
        }

        auto pIter = m_listAllPos.begin();
        for(int i=0; i<nIndex;++i,++pIter){}
        m_listAllPos.erase(pIter);

        NeedUpdate();
        return(true);
    }

    /**
     * @brief更新点
     * @return
     */
    bool UpdatePoint(int nIndex, const ScenePos &rPos)
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
            NeedUpdate();
        }

        return(true);
    }

    /**
     * @brief 一次设置多个点
     */
    void SetMultPos(const std::vector<ScenePos> & vAllPoints)
    {
        m_listAllPos.clear();

        for(auto one : vAllPoints)
        {
            m_listAllPos.push_back(one);
        }

        NeedUpdate();
    }

    /**
     * @brief 获取位置点
     * @return
     */
    std::vector<ScenePos> GetMulPos()const
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

    /**
     * @brief 获取个数
     * @return
     */
    int GetCount()const
    {
        return(m_listAllPos.size());
    }

    /**
     * @brief GetPoint
     * @return
     */
    ScenePos GetPoint(int nIndex)const
    {
        if(nIndex<0 || nIndex >= m_listAllPos.size())
        {
            return(ScenePos());
        }

        auto pIter = m_listAllPos.begin();
        for(int i=0; i<nIndex;++i,++pIter){}

        return(*pIter);
    }

    /**
     * @brief 清空所有的点
     */
    void Clear()
    {
        m_listAllPos.clear();
        NeedUpdate();
    }
protected:

    void NeedUpdate() SET_TRUE_SHAPE_UPDATE(m_bCountChanged)

    void UpdateShape()
    {
        if(m_bCountChanged)
        {
            ImplSceneShape<T>::m_pVertexArray->resize(m_listAllPos.size());

            std::vector<ScenePos> vAllConverdPos;
            int nIndex=0;
            if(nullptr != T::m_pDealPoint && T::m_pDealPoint->Conversion(m_listAllPos,vAllConverdPos))
            {
                for(auto one : vAllConverdPos)
                {
                    ImplSceneShape<T>::m_pVertexArray->at(nIndex++).set(one.fX,one.fY,one.fZ);
                }
            }
            else
            {
                for(auto one : m_listAllPos)
                {
                    ImplSceneShape<T>::m_pVertexArray->at(nIndex++).set(one.fX,one.fY,one.fZ);
                }
            }
            m_pDrawArrays->setCount(ImplSceneShape<T>::m_pVertexArray->size());
            m_bCountChanged=false;
        }
    }
protected:
    bool                   m_bCountChanged=false;
    std::list<ScenePos>    m_listAllPos;
    osg::ref_ptr<osg::DrawArrays> m_pDrawArrays;
};

#endif // IMPL_SCENE_SHAPE_H
