#ifndef INTERFACE_OSG_SCENE_NODE_H
#define INTERFACE_OSG_SCENE_NODE_H
#include <osg/Node>

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
     * @brief 判断是否需要更新
     * @return
     */
    bool NeedUpdate(){return(m_bNeedUpdate);}
protected:

    /**
     * @brief 节点更改消息
     */
    void NodeChanged()
    {
        m_bNeedUpdate = true;
    }

    class COsgSceneNodeUpdateCallback:public osg::Callback
    {
    public:
        COsgSceneNodeUpdateCallback(IOsgSceneNode* pSceneNode):m_pSceneNode(pSceneNode){}

        /// 回调函数
        bool run(osg::Object* object, osg::Object* data)
        {
            if(m_pSceneNode->NeedUpdate())
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
        m_pUpdateCallBack = new COsgSceneNodeUpdateCallback(this);
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
    bool                    m_bInit=false;
    bool                    m_bCanDelete=true;
    bool                    m_bNeedUpdate=false;
};
#endif
