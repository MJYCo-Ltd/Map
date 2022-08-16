#ifndef INTERFACE_SPACE_ENV_HEARDER_H
#define INTERFACE_SPACE_ENV_HEARDER_H

#include <Plot/Common/ISceneAttitudeGroup.h>
class ISpaceBackGround;

/**
 * @brief 场景节点类
 */
class ISpaceEnv:public ISceneAttitudeGroup
{
public:

    CONSTRUCTOR(ISpaceEnv,ISceneAttitudeGroup)
    /**
     * @brief 获取星空背景
     * @return
     */
    virtual ISpaceBackGround* GetSpaceBackGround()=0;

    /**
     * @brief 控制星空背景是否显示
     * @attention 默认显示
     */
    virtual void ShowSpaceBackGround(bool)=0;

    /**
     * @brief 获取地固系下太阳位置
     * @return
     */
    virtual const Math::CVector& GetSunPos()=0;
};

#endif
