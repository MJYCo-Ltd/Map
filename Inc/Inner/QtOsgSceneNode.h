#ifndef QT_OSG_SCENE_NODE_H
#define QT_OSG_SCENE_NODE_H

#include <set>
#include <osg/Group>
#include <algorithm>
#include <osg/OperationThread>

#include <Inner/IRender.h>
#include <Inner/OsgExtern/OsgExtern.h>

#include "IOsgSceneNode.h"
#include "OsgExtern/OsgExtern_Global.h"
#include "SceneGraph/ISceneGraph.h"
using namespace std;
class QtSceneGraph;

template <typename T>
class QtOsgSceneNode:public T,public IOsgSceneNode
{
public:
    explicit QtOsgSceneNode(ISceneGraph* pSceneGraph)
    {
        T::m_pSceneGraph = pSceneGraph;
    }

    virtual ~QtOsgSceneNode()
    {
        T::m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new RemoveFromeScene(m_pOsgNode));
        m_pOsgNode = nullptr;
    }

    /**
     * @brief 设置场景节点的位置
     */
    void SetPos(const ScenePos&rPos)
    {
        if(m_unScenePos != rPos)
        {
            m_unScenePos = rPos;
            PosChanged();
        }
    }

    /**
     * @brief 获取当前节点位置
     * @return 当前节点位置
     */
    const ScenePos& GetPos()
    {
        return(m_unScenePos);
    }

    /**
     * @brief 添加场景节点
     */
    bool AddSceneNode(ISceneNode*rSceneNode)
    {
        if(nullptr == rSceneNode)
        {
            return(false);
        }

        /// 如果两者不是同一个渲染器,则返回
        if(rSceneNode->GetBindSceneGraph() != T::m_pSceneGraph)
        {
            return(false);
        }

        IOsgSceneNode* pOsgSceneNode = dynamic_cast<IOsgSceneNode*>(rSceneNode);

        auto findItor = m_setChildNode.find(pOsgSceneNode);
        if(findItor == m_setChildNode.end())
        {
            auto update = new CModifyNode(m_pOsgNode,pOsgSceneNode->GetOsgNode(),true);
            T::m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(update);

            m_setChildNode.insert(pOsgSceneNode);
            return(true);
        }

        return(false);
    }

    /**
     * @brief 删除场景节点
     * @return 是否删除节点
     */
    bool RemoveSceneNode(ISceneNode* rSceneNode)
    {
        IOsgSceneNode* pOsgSceneNode = dynamic_cast<IOsgSceneNode*>(rSceneNode);
        auto findItor = m_setChildNode.find(pOsgSceneNode);
        if(findItor != m_setChildNode.end())
        {
            auto update = new CModifyNode(m_pOsgNode,pOsgSceneNode->GetOsgNode(),false);
            T::m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(update);

            m_setChildNode.erase(findItor);
            return(true);
        }

        return(false);
    }

    /**
     * @brief 初始化场景节点
     */
    void InitSceneNode()
    {
        m_pOsgNode = new osg::Group;
    }

    /**
     * @brief 获取osg的节点
     * @return
     */
    osg::Group* GetOsgNode()
    {
        return(m_pOsgNode.get());
    }

    /**
     * @brief 设置节点是否可见
     * @param bVisible
     */
    void SetVisible(bool bVisible)
    {
        if(m_bVisible != bVisible)
        {
            m_bVisible = bVisible;
            VisibleChanged();
        }
    }
    bool IsVisible(){return(m_bVisible);}

protected:
    virtual void VisibleChanged(){}
    /**
     * @brief 位置更改
     */
    virtual void PosChanged(){}

protected:
    set<IOsgSceneNode*>               m_setChildNode;/// 子节点
    osg::ref_ptr<osg::Group>          m_pOsgNode;    /// 本节点
    ScenePos                          m_unScenePos;  /// 场景位置
    bool                              m_bVisible=true;/// 是否可见
};

#endif // QT_OSG_SCENE_NODE_H
