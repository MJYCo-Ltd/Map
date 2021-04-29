#ifndef IMPL_SCENE_NODE_H
#define IMPL_SCENE_NODE_H
#include <iostream>
#include <osg/Depth>
#include <osgEarth/VirtualProgram>
#include <osgEarth/Registry>
#include <osgEarth/ObjectIndex>
#include <osgEarth/GLUtils>
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
        if(m_pRootNode.valid())
        {
            m_pRootNode->removeUpdateCallback(m_pUpdateCallBack);
        }

        if(nullptr != pNode)
        {
            m_pRootNode = pNode;
            m_preMask = m_pRootNode->getNodeMask();
            m_pRootNode->addUpdateCallback(m_pUpdateCallBack);

            LightChanged();
        }
    }

    /**
     * @brief 节点更新回调
     */
    void UpdateNode()
    {
        if(m_bPickStateChanged)
        {
            if(T::m_bCanPick)
            {
                T::m_unID = osgEarth::Registry::instance()->getObjectIndex()->tagNode(m_pRootNode.get(),m_pRootNode.get());
                if(T::m_bVisible)
                {
                    m_pRootNode->setNodeMask(PICK_MASK);
                }
            }
            else
            {
                if(T::m_bVisible)
                {
                    m_pRootNode->setNodeMask(m_preMask);
                }
                else
                {
                    m_pRootNode->setNodeMask(0);
                }
                osgEarth::Registry::instance()->getObjectIndex()->remove(T::m_unID);
                T::m_unID = 0;
            }
            m_bPickStateChanged=false;
        }

        /// 如果光照可见
        if(m_bLightingChanged)
        {
            osgEarth::GLUtils::setLighting(m_pRootNode->getOrCreateStateSet(),
                                           T::m_bOpenLight ? osg::StateAttribute::ON : osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
            m_bLightingChanged=false;
        }

        IOsgSceneNode::UpdateNode();
    }

    /// 重写子类的函数
    void VisibleChanged()
    {
        if(T::m_bVisible)
        {
            if(T::m_bCanPick)
            {
                m_pRootNode->setNodeMask(PICK_MASK);
            }
            else
            {
                m_pRootNode->setNodeMask(m_preMask);
            }
        }
        else
        {
            m_pRootNode->setNodeMask(0);
        }

        /// 更改显隐状态
        for(auto one : m_allAttach)
        {
            one->NodeVisibleChanged(T::m_bVisible);
        }
    }

    void PickStateChanged()SET_TRUE_NODE_UPDATE(m_bPickStateChanged)
    void LightChanged()SET_TRUE_NODE_UPDATE(m_bLightingChanged)

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
    bool m_bPickStateChanged=false;
    bool m_bLightingChanged=false;
};

#endif // IMPL_SCENE_NODE_H
