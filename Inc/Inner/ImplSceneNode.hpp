#ifndef IMPL_SCENE_NODE_H
#define IMPL_SCENE_NODE_H

#include <osgEarth/Registry>
#include <osgEarth/ObjectIndex>
#include <osgEarth/GLUtils>
#include <Inner/OsgExtern/OsgExtern.h>
#include <Inner/IOsgSceneNode.h>
#include <Inner/IRender.h>
#include <Inner/ILoadResource.h>
#include <SceneGraph/ISceneGraph.h>
#include <SceneGraph/SceneType.h>
#include <SceneGraph/INodeAbility.h>
#include <Inner/OsgExtern/NodeAbilityManager.h>
/**
 *  实现ISceneNode所有的接口
 */
template <typename T>
class ImplSceneNode:public T,public IOsgSceneNode,public RenderCall
{
public:
    CONSTRUCTOR(ImplSceneNode,T)

    /**
     * @brief 释放占用的空间
     */
    virtual ~ImplSceneNode()
    {
        T::m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new RemoveFromeScene(m_pProgramNode));
        m_pProgramNode = nullptr;
    }

    /**
     * @brief 转换成OSG的指针
     * @return
     */
    IOsgSceneNode* AsOsgSceneNode(){return(this);}

protected:

    /// 初始化节点
    virtual void InitNode()
    {
        IOsgSceneNode::InitNode();
        m_pProgramNode->setStateSet(
                    T::m_pSceneGraph->ResouceLoader()->GetOrCreateStateSet(m_unStateSet));
        m_bCallOne=true;
    }

    /**
     * @brief 设置节点
     * @param pNode
     */
    void SetOsgNode(osg::Node* pNode)
    {
        if(m_pRootNode.valid())
        {
            m_pProgramNode->removeChild(m_pRootNode.get());
            pNode->setNodeMask(m_preMask);
        }
        else
        {
            m_preMask = pNode->getNodeMask();
        }

        m_pProgramNode->addChild(pNode);

        m_pRootNode = pNode;
    }

    /**
     * @brief 合并状态集合
     */
    void MergeStateSet(unsigned uState)
    {
        unsigned unPreStateSet = m_unStateSet;
        m_unStateSet |= uState;
        if(unPreStateSet != m_unStateSet)
        {
            m_bStateChanged=true;
            NodeChanged();
        }
    }

    /**
     * @brief 移除状态集合
     */
    void RemoveStateSet(unsigned uState)
    {
        unsigned unPreStateSet = m_unStateSet;
        m_unStateSet &= ~uState;
        if(unPreStateSet != m_unStateSet)
        {
            m_bStateChanged=true;
            NodeChanged();
        }
    }

    unsigned& StateSet()
    {
        return(m_unStateSet);
    }


    /**
     * @brief 节点更新回调
     */
    void FrameCall()
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
            if(T::m_bOpenLight)
            {
                MergeStateSet(LIGHTING_STATE);
            }
            else
            {
                RemoveStateSet(LIGHTING_STATE);
            }
            m_bLightingChanged=false;
        }

        /// 是否一直显示
        if(m_bShowTopChanged)
        {
            //移动目标闪烁版本
            m_pRootNode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST,
                                                        T::m_bShowTop ? osg::StateAttribute::OFF : osg::StateAttribute::ON);

            m_bShowTopChanged=false;
        }

        /// 如果能力状态更改
        if(m_bAbilityChanged)
        {
            for(auto one : m_mapAbility)
            {
                one.second->UpdateAbility();
            }
            m_bAbilityChanged=false;
        }

        /// 渲染状态修改
        if(m_bStateChanged)
        {
            m_pProgramNode->setStateSet(T::m_pSceneGraph->ResouceLoader()->GetOrCreateStateSet(m_unStateSet));
            m_bStateChanged = false;
        }
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

    void CullChanged()
    {
        if(T::m_bOpenCull)
        {
            m_pProgramNode->setCullingActive(true);
        }
        else
        {
            m_pProgramNode->setCullingActive(false);
        }
    }

    void NodeChanged()
    {
        T::m_pSceneGraph->SceneGraphRender()->SubMessage(this);
    }

    void PickStateChanged()SET_TRUE_NODE_UPDATE(m_bPickStateChanged)
    void LightChanged()SET_TRUE_NODE_UPDATE(m_bLightingChanged)
    void ShowTopChanged()SET_TRUE_NODE_UPDATE(m_bShowTopChanged)
    void AbilityChanged() SET_TRUE_NODE_UPDATE(m_bAbilityChanged)

    void AddNode(osg::Group* pGroup,osg::Node* pNode)
    {
        T::m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CModifyNode(pGroup,pNode,true));
    }

    void DelNode(osg::Group* pGroup,osg::Node* pNode)
    {
        T::m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CModifyNode(pGroup,pNode,false));
    }


    ///创建一个能力
    INodeAbility* GetOrCreateAbility(ABILITY_TYPE type)
    {
        INodeAbility* pReturn{nullptr};
        if(type & FLASH_ABILITY)
        {
            auto findOne = m_mapAbility.find(FLASH_ABILITY);
            if(m_mapAbility.end() != findOne)
            {
                pReturn = findOne->second;
            }
            else
            {
                pReturn = CNodeAbilityManager::GetInstance()->CreateAbility(FLASH_ABILITY);
                pReturn->BoundNode(this);
                m_mapAbility.insert(std::make_pair(FLASH_ABILITY,pReturn));
            }
        }

        if(type & FLAT_ABILITY)
        {
            auto findOne = m_mapAbility.find(FLAT_ABILITY);
            if(m_mapAbility.end() != findOne)
            {
                pReturn = findOne->second;
            }
            else
            {
                pReturn = CNodeAbilityManager::GetInstance()->CreateAbility(FLAT_ABILITY);
                pReturn->BoundNode(this);
                m_mapAbility.insert(std::make_pair(FLAT_ABILITY,pReturn));
            }
        }
        return(pReturn);
    }

    /// 移除能力
    bool RemoveAbility(ABILITY_TYPE type)
    {
        if(T::m_emAbility & type)
        {
            return (true);
        }
        else
        {
            return(false);
        }
    }

protected:
    osg::Node::NodeMask  m_preMask{-1u};
    bool m_bPickStateChanged{false};
    bool m_bLightingChanged{false};
    bool m_bShowTopChanged{false};
    bool m_bAbilityChanged{false};
    bool m_bStateChanged{false};
    std::map<ABILITY_TYPE,INodeAbility*> m_mapAbility; /// 根据能力类型创建能力
};

#endif // IMPL_SCENE_NODE_H
