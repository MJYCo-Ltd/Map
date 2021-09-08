#ifndef IMPL_SCENE_GEOMETRY_H
#define IMPL_SCENE_GEOMETRY_H

#include <Inner/ImplSceneShape.hpp>
#include <OpenThreads/Mutex>

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
        bool bUnlock = 0 == m_listMutex.lock();

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

        if(bUnlock)
        {
            m_listMutex.unlock();
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

        bool bUnlock = 0 == m_listMutex.lock();
        auto pIter = m_listAllPos.begin();
        for(int i=0; i<nIndex;++i,++pIter){}
        m_listAllPos.erase(pIter);

        if(bUnlock)
        {
            m_listMutex.unlock();
        }

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

        bool bUnlock = 0 == m_listMutex.lock();

        auto pIter = m_listAllPos.begin();
        for(int i=0; i<nIndex;++i,++pIter){}

        if(*pIter != rPos)
        {
            *pIter = rPos;
            NeedUpdate();
        }

        if(bUnlock)
        {
            m_listMutex.unlock();
        }

        return(true);
    }

    /**
     * @brief 一次设置多个点
     */
    void SetMultPos(const std::vector<ScenePos> & vAllPoints)
    {
        bool bUnlock = 0 == m_listMutex.lock();
        m_listAllPos.clear();

        for(auto one : vAllPoints)
        {
            m_listAllPos.push_back(one);
        }

        if(bUnlock)
        {
            m_listMutex.unlock();
        }

        NeedUpdate();
    }

    /**
     * @brief 获取位置点
     * @return
     */
    std::vector<ScenePos> GetMulPos()
    {
        std::vector<ScenePos> vTempPos;
        vTempPos.resize(m_listAllPos.size());
        int nIndex(0);

        bool bUnlock = 0 == m_listMutex.lock();
        for(auto one : m_listAllPos)
        {
            ScenePos& rPos = vTempPos.at(nIndex++);

            rPos.dX = one.dX;
            rPos.dY = one.dY;
            rPos.dZ = one.dZ;
        }

        if(bUnlock)
        {
            m_listMutex.unlock();
        }

        return(vTempPos);
    }

    /**
     * @brief 获取所有点的位置
     * @return
     */
    const std::list<ScenePos>& BetterGetMulPos()const
    {
        return(m_listAllPos);
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
    ScenePos GetPoint(int nIndex)
    {
        ScenePos tmpPos;
        if(nIndex<0 || nIndex >= m_listAllPos.size())
        {
            return(tmpPos);
        }

        bool bUnlock = 0 == m_listMutex.lock();
        auto pIter = m_listAllPos.begin();
        for(int i=0; i<nIndex;++i,++pIter){}
        tmpPos = *pIter;
        if(bUnlock)
        {
            m_listMutex.unlock();
        }

        return(tmpPos);
    }

    /**
     * @brief 清空所有的点
     */
    void Clear()
    {
        bool bUnlock = 0 == m_listMutex.lock();
        m_listAllPos.clear();
        if(bUnlock)
        {
            m_listMutex.unlock();
        }

        NeedUpdate();
    }
protected:

    /**
     * @brief 外部调用强制更新
     */
    void NeedUpdate() SET_TRUE_SHAPE_UPDATE(m_bCountChanged)

    /**
     * @brief 更新点信息
     */
    void UpdateVetrix()
    {
        bool bUnlock = 0 == m_listMutex.lock();
        ImplSceneShape<T>::m_pVertexArray->resize(m_listAllPos.size());

        int nIndex=0;

        std::vector<ScenePos> vAllConverdPos;
        if(nullptr != T::m_pDealPoint && T::m_pDealPoint->Conversion(m_listAllPos,vAllConverdPos))
        {
            for(auto one : vAllConverdPos)
            {
                ImplSceneShape<T>::m_pVertexArray->at(nIndex++).set(one.dX,one.dY,one.dZ);
            }
        }
        else
        {
            for(auto one : m_listAllPos)
            {
                ImplSceneShape<T>::m_pVertexArray->at(nIndex++).set(one.dX,one.dY,one.dZ);
            }
        }
        if(bUnlock)
        {
            m_listMutex.unlock();
        }
    }

    void UpdateShape()
    {
        if(m_bCountChanged)
        {
            UpdateVetrix();
            if(m_pDrawArrays.valid())
            {
                m_pDrawArrays->setCount(ImplSceneShape<T>::m_pVertexArray->size());
            }

            m_bCountChanged=false;
        }
    }
protected:
    bool                   m_bCountChanged{false};
    OpenThreads::Mutex     m_listMutex;
    std::list<ScenePos>    m_listAllPos;
    osg::ref_ptr<osg::DrawArrays> m_pDrawArrays;
};

#endif // IMPL_SCENE_SHAPE_H
