#ifndef INTERFACE_OSG_SCENE_NODE_H
#define INTERFACE_OSG_SCENE_NODE_H
#include <osg/Group>
#include <osg/Depth>
#include <osg/PolygonMode>

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

enum STATESET_TYPE
{
    BLEND_STATE,
    LINE_STATE,
    FACE_STATE
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
     * @brief 初始化节点
     */
    virtual void InitNode(){m_pProgramNode=new osg::Group;}
    /**
     * @brief 获取显示节点
     * @return
     */
    osg::Node* GetOsgNode(){return(m_pRootNode);}

    /**
     * @brief 获取真正的根节点
     * @return
     */
    virtual osg::Node* GetRealNode(){return(m_pProgramNode);}

    /**
     * @brief 设置是否可以定时删除
     * @param bTimerDelete
     */
    void SetCanDelete(bool bTimerDelete){m_bCanDelete = bTimerDelete;}

    /**
     * @brief 判断节点是可以删除
     * @return
     */
    bool CanDelete() const{return(m_bCanDelete && m_bInit && m_pProgramNode->referenceCount()<2);}

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
     * @brief 获取或者创建指定的渲染指示器
     * @return
     */
    osg::StateSet* GetOrCreateStateSet(STATESET_TYPE type)
    {
        auto pFindone = s_globalStateSets.find(type);
        if(s_globalStateSets.end() != pFindone)
        {
            return(pFindone->second);
        }
        else
        {
            osg::StateSet* pStateSet{nullptr};

            switch (type)
            {
            case BLEND_STATE:
            {
                pStateSet= new osg::StateSet;
                pStateSet->setMode(GL_BLEND,osg::StateAttribute::ON);
                auto pDepth = new osg::Depth;
                pDepth->setWriteMask(false);
                pStateSet->setAttribute(pDepth);
                pStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
            }
                break;
            case FACE_STATE:
            {
                pStateSet= new osg::StateSet;
                pStateSet->setAttributeAndModes(
                new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::FILL));
            }
                break;
            case LINE_STATE:
            {
                pStateSet= new osg::StateSet;
                pStateSet->setAttributeAndModes(
                new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE));
            }
                break;
            }

            if(nullptr != pStateSet)
            {
                s_globalStateSets[type] = pStateSet;
            }
            return(pStateSet);
        }
    }

protected:

    /**
     * @brief 对节点,进行线程安全的操作
     */
    virtual void AddNode(osg::Group* pGroup,osg::Node* pNode)=0;
    virtual void DelNode(osg::Group* pGroup,osg::Node* pNode)=0;
	
protected:
    bool                       m_bInit{false};
    osg::ref_ptr<osg::Node>    m_pRootNode;    /// 真实的节点
    osg::ref_ptr<osg::Group>   m_pProgramNode; /// 用于设置着色器程序的节点
    std::set<ISceneNodeAttach*> m_allAttach;   /// 依附的点
    bool                       m_bCanDelete{true};
    static std::map<STATESET_TYPE,osg::ref_ptr<osg::StateSet>> s_globalStateSets;
};
#endif
