#ifndef IMPL_SCENE_GEOMETRY_H
#define IMPL_SCENE_GEOMETRY_H

#include <Inner/ImplSceneShape.hpp>
#include <Inner/ThreadSafety/SafetyList.h>

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
        m_listAllPos.Add(nIndex,rScenePos);
        NeedUpdate();
    }

    void AddPoint(const ScenePos& rScenePos)
    {
        m_listAllPos.Add(rScenePos);
        NeedUpdate();
    }

    /**
     * @brief 移除一个点
     * @return
     */
    bool RemovePoint(int nIndex)
    {
        if(nIndex<0 || nIndex >= m_listAllPos.Size())
        {
            return(false);
        }

        m_listAllPos.RemoveIndex(nIndex);
        NeedUpdate();
        return(true);
    }

    /**
     * @brief更新点
     * @return
     */
    bool UpdatePoint(int nIndex, const ScenePos &rPos)
    {
        if(nIndex<0 || nIndex >= m_listAllPos.Size())
        {
            return(false);
        }

        m_listAllPos.Update(nIndex,rPos);
        NeedUpdate();
        return(true);
    }

    /**
     * @brief 一次设置多个点
     */
    void SetMultPos(const std::vector<ScenePos> & vAllPoints)
    {
        if(vAllPoints.size() < 1)
        {
            Clear();
        }
        else
        {
            m_listAllPos.Update(vAllPoints);
            NeedUpdate();
        }
    }

    /**
     * @brief 获取位置点
     * @return
     */
    std::vector<ScenePos> GetMulPos() const
    {
        std::vector<ScenePos> vTempPos;
        m_listAllPos.Get(vTempPos);

        return(vTempPos);
    }

    /**
     * @brief 获取个数
     * @return
     */
    int GetCount()const
    {
        return(m_listAllPos.Size());
    }

    /**
     * @brief GetPoint
     * @return
     */
    const ScenePos& GetPoint(int nIndex) const
    {
        static ScenePos s_Pos;
        if(nIndex<0 || nIndex >= m_listAllPos.Size())
        {
            memset(&s_Pos,0,sizeof(s_Pos));

            return(s_Pos);
        }
        m_listAllPos.Get(nIndex,s_Pos);
        return(s_Pos);
    }

    /**
     * @brief 清空所有的点
     */
    void Clear()
    {
        if(0 != m_listAllPos.Size())
        {
            m_listAllPos.Clear();
            NeedUpdate();
        }
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
        int nIndex{0};
        std::vector<ScenePos> vSourcePos;

        if(nullptr != T::m_pDealPoint)
        {
            /// 首先通知形状更改，查看是否需要进行修改
            T::m_pDealPoint->ShapeChanged();

            m_listAllPos.Get(vSourcePos);

            std::vector<ScenePos> vAllConverdPos(vSourcePos.size());
            if(T::m_pDealPoint->Conversion(vSourcePos,vAllConverdPos))
            {
                ImplSceneShape<T>::m_pVertexArray->resize(vSourcePos.size());
                ImplSceneShape<T>::m_pNormals->resize(vSourcePos.size());

                osg::Vec3 vPos;
                for(auto one : vAllConverdPos)
                {
                    ImplSceneShape<T>::m_pVertexArray->at(nIndex).set(one.dX,one.dY,one.dZ);
                    vPos.set(one.dX,one.dY,one.dZ);
                    vPos.normalize();
                    ImplSceneShape<T>::m_pNormals->at(nIndex).set(vPos);
                    ++nIndex;
                }

                return;
            }
        }

        /// 不更改的
        {
            m_listAllPos.Get(vSourcePos);
            ImplSceneShape<T>::m_pVertexArray->resize(vSourcePos.size());
            ImplSceneShape<T>::m_pNormals->resize(vSourcePos.size());
            osg::Vec3 vPos;
            for(auto one : vSourcePos)
            {
                vPos.set(one.dX,one.dY,one.dZ);
                ImplSceneShape<T>::m_pVertexArray->at(nIndex).set(one.dX,one.dY,one.dZ);
                vPos.normalize();
                ImplSceneShape<T>::m_pNormals->at(nIndex).set(vPos);
                ++nIndex;
            }
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
    bool                     m_bCountChanged{false};
    CSafetyList<ScenePos>    m_listAllPos;
    osg::ref_ptr<osg::DrawArrays> m_pDrawArrays;
};

#endif // IMPL_SCENE_SHAPE_H
