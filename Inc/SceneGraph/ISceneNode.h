#ifndef INTERFACE_SCENE_NODE_HEARDER_H
#define INTERFACE_SCENE_NODE_HEARDER_H

#include "SceneType.h"

class ISceneGraph;
/**
 * @brief 场景节点类
 */
class ISceneNode
{
public:
    virtual ~ISceneNode(){}

    /**
     * @brief 设置场景节点的位置
     */
    virtual void SetPos(const ScenePos&)=0;
    virtual const ScenePos& GetPos() const=0;

    /**
     * @brief 设置姿态
     * @attention 只有ScenePos的bIsGeo才可以进行姿态调整
     */
    virtual void SetAttitude(const SceneAttitude&)=0;
    virtual const SceneAttitude& GetAttitude()const =0;

    /**
     * @brief 添加场景节点
     * @attention 只有一个线程下的节点才能相互添加
     */
    virtual bool AddSceneNode(ISceneNode*)=0;

    /**
     * @brief 删除场景节点
     * @attention 移除场景节点
     */
    virtual bool RemoveSceneNode(ISceneNode*)=0;

    /**
     * @brief 设置节点可见
     */
    virtual void SetVisible(bool)=0;

    /**
     * @brief 判断节点是否可见
     * @return
     */
    virtual bool IsVisible() const=0;

    /**
     * @brief 是否可以删除
     * @return
     */
    virtual bool CanDelete() const =0;

    /**
     * @brief 获取绑定的场景
     * @return
     */
    virtual ISceneGraph* GetBindSceneGraph(){return(m_pSceneGraph);}
protected:
    ISceneGraph* m_pSceneGraph;
};

#endif
