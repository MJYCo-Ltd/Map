#include <algorithm>
#include <osg/OperationThread>

#include <Inner/IRender.h>
#include <Inner/OsgExtern/OsgExtern.h>

#include "QtSceneNode.h"


template <typename T>
QtSceneNode<T>::QtSceneNode(ISceneGraph *pSceneGraph)
{
    m_pSceneGraph = pSceneGraph;
}

/// 析构函数
template <typename T>
QtSceneNode<T>::~QtSceneNode()
{
    static int nSize=0;
    m_pOsgNode = nullptr;
}

/// 设置位置
template <typename T>
void QtSceneNode<T>::SetPos(const ScenePos & rPos)
{
    if(m_unScenePos != rPos)
    {
        m_unScenePos = rPos;
        PosChanged();
    }
}

/// 获取节点位置
template <typename T>
const ScenePos &QtSceneNode<T>::GetPos()
{
    return(m_unScenePos);
}

/// 添加节点
template <typename T>
bool QtSceneNode<T>::AddSceneNode(ISceneNode* rSceneNode)
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

    auto findItor = m_setChildNode.find(rSceneNode);
    if(findItor == m_setChildNode.end())
    {
        auto update = new CModifyNode(m_pOsgNode,rSceneNode->GetOsgNode(),true);
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(update);

        m_setChildNode.insert(rSceneNode);
        return(true);
    }

    return(false);
}

/// 删除一个子节点
template <typename T>
bool QtSceneNode<T>::RemoveSceneNode(ISceneNode* rSceneNode)
{
    auto findItor = m_setChildNode.find(rSceneNode);
    if(findItor != m_setChildNode.end())
    {
        auto update = new CModifyNode(m_pOsgNode,rSceneNode->GetOsgNode(),false);
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(update);

        m_setChildNode.erase(findItor);
        return(true);
    }

    return(false);
}

template<typename T>
osg::Node* QtSceneNode<T>::GetOsgNode()
{
    return(m_pOsgNode.get());
}


template<typename T>
void QtSceneNode<T>::Clear()
{
    m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new RemoveFromeScene(m_pOsgNode));
}

template<typename T>
void QtSceneNode<T>::SetVisible(bool bVisible)
{
    if(m_bVisible != bVisible)
    {
        m_bVisible = bVisible;
        VisibleChanged();
    }
}

template<typename T>
void QtSceneNode<T>::InitSceneNode()
{
    m_pOsgNode = new osg::Group;
}
