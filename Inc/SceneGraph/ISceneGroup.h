#ifndef INTERFACE_SCENE_GROUP_HEARDER_H
#define INTERFACE_SCENE_GROUP_HEARDER_H

#include <SceneGraph/ISceneNode.h>

/**
 * @brief 场景组节点
 */
class ISceneGroup:public ISceneNode
{
public:
    CONSTRUCTOR(ISceneGroup,ISceneNode)

    /**
     * @brief
     * @return
     */
    ISceneGroup* AsSceneGroup(){return(this);}

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
protected:
    virtual ~ISceneGroup(){}
};

#endif//INTERFACE_SCENE_GROUP_HEARDER_H
