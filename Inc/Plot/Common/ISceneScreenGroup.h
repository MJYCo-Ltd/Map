#ifndef INTERFACE_SCENE_SCREEN_GROUP_HEARDER_H
#define INTERFACE_SCENE_SCREEN_GROUP_HEARDER_H

#include <SceneGraph/ISceneGroup.h>

/**
 * @brief 场景节点类
 */
class ISceneScreenGroup:public ISceneGroup
{
public:
    CONSTRUCTOR(ISceneScreenGroup,ISceneGroup)

    /**
     * @brief 转换成本类型
     * @return
     */
    ISceneScreenGroup* AsSceneScreenGroup(){return(this);}

protected:
    virtual ~ISceneScreenGroup(){}
protected:
};

#endif //INTERFACE_SCENE_FLASH_GROUP_HEARDER_H
