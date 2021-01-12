#ifndef IMPL_SCENE_NODE_H
#define IMPL_SCENE_NODE_H
#include <iostream>
#include <osg/Depth>
#include <osgEarth/VirtualProgram>
#include <Inner/OsgExtern/OsgExtern.h>
#include <Inner/IOsgSceneNode.h>
#include <Inner/IRender.h>
#include <Inner/ILoadResource.h>
#include <SceneGraph/ISceneGraph.h>
#include <SceneGraph/SceneType.h>

/**
 *  实现ISceneNode所有的接口
 */
template <typename T>
class ImplSceneNode:public T,public IOsgSceneNode
{
public:
    CONSTRUCTOR(ImplSceneNode,T)

    /**
     * @brief 释放占用的空间
     */
    virtual ~ImplSceneNode()
    {
        T::m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new RemoveFromeScene(m_pRootNode));
        m_pRootNode = nullptr;
    }

    /**
     * @brief 转换成OSG的指针
     * @return
     */
    IOsgSceneNode* AsOsgSceneNode(){return(this);}

protected:
    /**
     * @brief 设置节点
     * @param pNode
     */
    void SetOsgNode(osg::Node* pNode)
    {
        if(nullptr != pNode)
        {
            m_pRootNode = pNode;
            m_pRootNode->addUpdateCallback(m_pUpdateCallBack);
        }
    }

    /// 重写子类的函数
    void VisibleChanged()
    {
        if(T::m_bVisible)
        {
            m_pRootNode->setNodeMask(m_preMask);
        }
        else
        {
            m_preMask = m_pRootNode->getNodeMask();
            m_pRootNode->setNodeMask(0);
        }

        /// 更改显隐状态
        for(auto one : m_allAttach)
        {
            one->NodeVisibleChanged(T::m_bVisible);
        }
    }

    void AddNode(osg::Group* pGroup,osg::Node* pNode)
    {
        T::m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CModifyNode(pGroup,pNode,true));
    }

    void DelNode(osg::Group* pGroup,osg::Node* pNode)
    {
        T::m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CModifyNode(pGroup,pNode,false));
    }

protected:
    osg::Node::NodeMask  m_preMask = 0xffffffffu;
};

#endif // IMPL_SCENE_NODE_H
