#ifndef IMPL_SCENE_GROUP_H
#define IMPL_SCENE_GROUP_H

#include <set>
#include "ImplSceneNode.hpp"
using namespace std;

/**
 *  实现ISceneNode所有的接口
 */
template <typename T>
class ImplSceneGroup:public ImplSceneNode<T>
{
public:
    CONSTRUCTOR(ImplSceneGroup,ImplSceneNode<T>)

    /// 添加节点
    bool AddSceneNode(ISceneNode* pSceneNode)
    {
        if (nullptr == pSceneNode)
        {
            return(false);
        }

        /// 如果两者不是同一个渲染器,则返回
        if (pSceneNode->GetBoundSceneGraph() != T::m_pSceneGraph)
        {
            return(false);
        }

        IOsgSceneNode* pOsgSceneNode = pSceneNode->AsOsgSceneNode();

        auto findItor = m_setChildNode.find(pOsgSceneNode);
        if (findItor == m_setChildNode.end())
        {
            AddNode(m_pGroup,pOsgSceneNode->GetOsgNode());
            m_setChildNode.insert(pOsgSceneNode);
            return(true);
        }

        return(false);
    }

    /// 移除节点
    bool RemoveSceneNode(ISceneNode* pSceneNode)
    {
        IOsgSceneNode* pOsgSceneNode = pSceneNode->AsOsgSceneNode();
        auto findItor = m_setChildNode.find(pOsgSceneNode);
        if (findItor != m_setChildNode.end())
        {
            DelNode(m_pGroup,pOsgSceneNode->GetOsgNode());

            m_setChildNode.erase(findItor);
            return(true);
        }

        return(false);
    }

protected:
    void InitNode()
    {
        ImplSceneNode<T>::InitNode();

        if(m_pRootNode.valid())
        {
            m_pGroup = m_pRootNode->asGroup();
        }
        else
        {
            m_pGroup = new osg::Group;
            SetOsgNode(m_pGroup);
        }
    }
protected:
    set<IOsgSceneNode*>       m_setChildNode;/// 子节点
    osg::Group*               m_pGroup;      /// 本节点
};

#endif // IMPL_SCENE_GROUP_H
