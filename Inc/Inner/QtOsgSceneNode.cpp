#include <algorithm>
#include <osg/OperationThread>

#include <Inner/IRender.h>
#include <Inner/OsgExtern/OsgExtern.h>

#include "QtOsgSceneNode.h"


template <typename T>
QtOsgSceneNode<T>::QtOsgSceneNode(ISceneGraph *pSceneGraph)
{
    m_pSceneGraph = pSceneGraph;
}

/// 析构函数
template <typename T>
QtOsgSceneNode<T>::~QtOsgSceneNode()
{
    m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new RemoveFromeScene(m_pOsgNode));
    m_pOsgNode = nullptr;
}

/// 设置位置
template <typename T>
void QtOsgSceneNode<T>::SetPos(const ScenePos & rPos)
{
    if(m_unScenePos != rPos)
    {
        m_unScenePos = rPos;
        PosChanged();
    }
}

/// 获取节点位置
template <typename T>
const ScenePos &QtOsgSceneNode<T>::GetPos()
{
    return(m_unScenePos);
}

/// 添加节点
template <typename T>
bool QtOsgSceneNode<T>::AddSceneNode(ISceneNode* rSceneNode)
{
    if(nullptr == rSceneNode)
    {
        return(false);
    }

    /// 如果两者不是同一个渲染器,则返回
    if(rSceneNode->GetBindSceneGraph() != m_pSceneGraph)
    {
        return(false);
    }

    IOsgSceneNode* pOsgSceneNode = dynamic_cast<IOsgSceneNode*>(rSceneNode);

    auto findItor = m_setChildNode.find(pOsgSceneNode);
    if(findItor == m_setChildNode.end())
    {
        auto update = new CModifyNode(m_pOsgNode,pOsgSceneNode->GetOsgNode(),true);
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(update);

        m_setChildNode.insert(pOsgSceneNode);
        return(true);
    }

    return(false);
}

/// 删除一个子节点
template <typename T>
bool QtOsgSceneNode<T>::RemoveSceneNode(ISceneNode* rSceneNode)
{
    IOsgSceneNode* pOsgSceneNode = dynamic_cast<IOsgSceneNode*>(rSceneNode);
    auto findItor = m_setChildNode.find(pOsgSceneNode);
    if(findItor != m_setChildNode.end())
    {
        auto update = new CModifyNode(m_pOsgNode,pOsgSceneNode->GetOsgNode(),false);
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(update);

        m_setChildNode.erase(findItor);
        return(true);
    }

    return(false);
}

template<typename T>
osg::Group *QtOsgSceneNode<T>::GetOsgNode()
{
    return(m_pOsgNode.get());
}

template<typename T>
void QtOsgSceneNode<T>::SetVisible(bool bVisible)
{
    if(m_bVisible != bVisible)
    {
        m_bVisible = bVisible;
        VisibleChanged();
    }
}

template<typename T>
void QtOsgSceneNode<T>::InitSceneNode()
{
    m_pOsgNode = new osg::Group;
}
