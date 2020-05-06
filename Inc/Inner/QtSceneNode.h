#ifndef QTSCENENODE_H
#define QTSCENENODE_H

#include <set>
#include <osg/Group>
#include "SceneGraph/ISceneNode.h"
#include "SceneGraph/ISceneGraph.h"
using namespace std;

class QtSceneGraph;

template <typename T>
class QtSceneNode:public T
{
public:
    explicit QtSceneNode(ISceneGraph* pSceneGraph);
    virtual ~QtSceneNode();

    /**
     * @brief 设置场景节点的位置
     */
    void SetPos(const ScenePos&rPos);

    /**
     * @brief 获取当前节点位置
     * @return 当前节点位置
     */
    const ScenePos& GetPos();

    /**
     * @brief 添加场景节点
     */
    bool AddSceneNode(ISceneNode*rSceneNode);

    /**
     * @brief 删除场景节点
     * @return 是否删除节点
     */
    bool RemoveSceneNode(ISceneNode* rSceneNode);

    /**
     * @brief 初始化场景节点
     */
    void InitSceneNode();

    /**
     * @brief 获取osg的节点
     * @return
     */
    osg::Node* GetOsgNode();

    /**
     * @brief 清空
     */
    void Clear();

    /**
     * @brief 设置节点是否可见
     * @param bVisible
     */
    void SetVisible(bool bVisible);
    bool IsVisible(){return(m_bVisible);}

protected:
    virtual void VisibleChanged(){}
    /**
     * @brief 位置更改
     */
    virtual void PosChanged(){}

protected:
    set<ISceneNode*>                  m_setChildNode;/// 子节点
    osg::ref_ptr<osg::Group>          m_pOsgNode;    /// 本节点
    ScenePos                          m_unScenePos;  /// 场景位置
    bool                              m_bVisible=true;/// 是否可见
};

//typedef QtSceneNode<ISceneNode> CSceneNode;

#include "QtSceneNode.cpp"

#endif // QTSCENENODE_H
