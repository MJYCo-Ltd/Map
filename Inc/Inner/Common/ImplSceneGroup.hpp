#ifndef IMPL_SCENE_GROUP_H
#define IMPL_SCENE_GROUP_H

#include <set>
#include <Inner/ImplSceneNode.hpp>

/**
 *  实现ISceneGroup所有的接口
 */
template <typename T>
class ImplSceneGroup:public ImplSceneNode<T>
{
public:
    CONSTRUCTOR(ImplSceneGroup,ImplSceneNode<T>)

    /// 添加节点
    bool AddSceneNode(ISceneNode* pSceneNode) override
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
            ImplSceneNode<T>::AddNode(m_pGroup.get(),pOsgSceneNode->GetOsgNode());
            pSceneNode->AsOsgSceneNode()->AddToGroup(m_pGroup.get());
            m_setChildNode.insert(pOsgSceneNode);
            InsertChildNode(pOsgSceneNode->GetOsgNode());
            return(true);
        }

        return(false);
    }

    /// 移除节点
    bool RemoveSceneNode(ISceneNode* pSceneNode) override
    {
        if (nullptr == pSceneNode)
        {
            return(false);
        }

        IOsgSceneNode* pOsgSceneNode = pSceneNode->AsOsgSceneNode();

        auto findItor = m_setChildNode.find(pOsgSceneNode);
        if (findItor != m_setChildNode.end())
        {
            pSceneNode->AsOsgSceneNode()->DelFromGroup(m_pGroup.get());
            RemoveChildNode(pOsgSceneNode->GetOsgNode());
            ImplSceneNode<T>::DelNode(m_pGroup.get(),pOsgSceneNode->GetOsgNode());

            m_setChildNode.erase(findItor);
            return(true);
        }

        return(false);
    }

protected:
    void InitNode() override
    {
        ImplSceneNode<T>::InitNode();
        m_pGroup = new osg::Group;
        ImplSceneNode<T>::SetOsgNode(m_pGroup.get());
    }

    void SetGroupNode(osg::Group* pGroup)
    {
        m_pGroup = pGroup;
        ImplSceneNode<T>::SetOsgNode(m_pGroup.get());
    }

    virtual void InsertChildNode(osg::Node*){}
    virtual void RemoveChildNode(osg::Node*){}
protected:
    std::set<IOsgSceneNode*>       m_setChildNode;/// 子节点
    osg::observer_ptr<osg::Group>m_pGroup;      /// 本节点
};

#endif // IMPL_SCENE_GROUP_H