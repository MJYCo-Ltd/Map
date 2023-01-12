#ifndef INTERFACE_SPACE_ENV_HEARDER_H
#define INTERFACE_SPACE_ENV_HEARDER_H
#include <Math/Matrix.h>
#include <SceneGraph/ISceneGroup.h>
class ISpaceBackGround;

/**
 * @brief 场景节点类
 */
class ISpaceEnv:public ISceneGroup
{
public:

    CONSTRUCTOR(ISpaceEnv,ISceneGroup)
    /**
     * @brief 获取星空背景
     * @return
     */
    virtual ISpaceBackGround* GetSpaceBackGround()=0;

    /**
     * @brief 获取地固系下太阳位置
     * @return
     */
    virtual const Math::CVector& GetSunPos()=0;
};

#endif
