#ifndef INTERFACE_OSG_SCENE_NODE_H
#define INTERFACE_OSG_SCENE_NODE_H
#include <osg/Node>

/**
 * @brief 场景依附节点
 */
class ISceneNodeAttach
{
public:
    ISceneNodeAttach(){}
    virtual ~ISceneNodeAttach(){}

    /**
     * @brief 显隐状态修改
     */
    virtual void NodeVisibleChanged(bool){}

    /**
     * @brief 增加到父节点
     */
    virtual void AddIntoParent(osg::Group*){}

    /**
     * @brief 从父节点移除
     */
    virtual void DelFromParent(osg::Group*){}
};

/**
 * @brief Osg类型的场景节点
 */
class IOsgSceneNode
{
public:
    IOsgSceneNode(){}
    virtual ~IOsgSceneNode(){}

    /**
     * @brief 初始化节点
     */
    void Init(){if(!m_bInit){m_bInit = true;InitNode();}}

    /**
     * @brief 获取显示节点
     * @return
     */
    osg::Node* GetOsgNode(){return(m_pRootNode);}

    /**
     * @brief 设置是否可以定时删除
     * @param bTimerDelete
     */
    void SetCanDelete(bool bTimerDelete){m_bCanDelete = bTimerDelete;}

    /**
     * @brief 判断节点是可以删除
     * @return
     */
    bool CanDelete() const{return(m_bCanDelete && m_bInit && m_pRootNode->referenceCount()<2);}

    /**
     * @brief 节点更改消息
     */
    void NodeChanged()
    {
        m_bNeedUpdate = true;
    }

    /**
     * @brief BindAttach
     */
    bool BindAttach(ISceneNodeAttach* pAttachNode)
    {
        if(m_allAttach.end() == m_allAttach.find(pAttachNode))
        {
            m_allAttach.insert(pAttachNode);
            return(true);
        }
        else
        {
            return(false);
        }
    }

    /**
     * @brief 移除依附者
     */
    bool RemoveAttach(ISceneNodeAttach* pAttachNode)
    {
        auto pFindOne = m_allAttach.find(pAttachNode);
        if(m_allAttach.end() != pFindOne)
        {
            m_allAttach.erase(pFindOne);
            return(true);
        }
        else
        {
            return(false);
        }
    }

    /**
     * @brief 添加到场景节点
     */
    void AddToGroup(osg::Group* pParent)
    {
        for(auto one : m_allAttach)
        {
            one->AddIntoParent(pParent);
        }
    }

    /**
     * @brief 从场景中移除节点
     */
    void DelFromGroup(osg::Group* pParent)
    {
        for(auto one : m_allAttach)
        {
            one->DelFromParent(pParent);
        }
    }

    /**
     * @brief 是否需要更新
     * @return
     */
    bool CanUpdate(){return(m_bNeedUpdate);}
protected:

    class COsgSceneNodeUpdateCallback:public osg::Callback
    {
    public:
        COsgSceneNodeUpdateCallback(IOsgSceneNode* pSceneNode):m_pSceneNode(pSceneNode){}

        /// 回调函数
        bool run(osg::Object* object, osg::Object* data)
        {
            if(m_pSceneNode->CanUpdate())
            {
                m_pSceneNode->UpdateNode();
            }
            return traverse(object, data);
        }
    private:
        IOsgSceneNode* m_pSceneNode;
    };

    /**
     * @brief 初始化节点
     */
    virtual void InitNode()
    {
        if(!m_pUpdateCallBack.valid())
        {
            m_pUpdateCallBack = new COsgSceneNodeUpdateCallback(this);
        }
    }

    /**
     * @brief 节点的更新操作
     */
    virtual void UpdateNode()
    {
        m_bNeedUpdate = false;
    }

    /**
     * @brief 对节点,进行线程安全的操作
     */
    virtual void AddNode(osg::Group* pGroup,osg::Node* pNode)=0;
    virtual void DelNode(osg::Group* pGroup,osg::Node* pNode)=0;
	
protected:
    osg::ref_ptr<osg::Node> m_pRootNode;
    osg::ref_ptr<COsgSceneNodeUpdateCallback> m_pUpdateCallBack;
    std::set<ISceneNodeAttach*> m_allAttach; ///依附的点
    bool                    m_bInit{false};
    bool                    m_bCanDelete{true};
    bool                    m_bNeedUpdate{false};
};
#endif
