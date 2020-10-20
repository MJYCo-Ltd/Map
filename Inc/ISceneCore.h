#ifndef INTERFACE_SCENE_CORE_H
#define INTERFACE_SCENE_CORE_H

#include "SceneCore_Global.h"
#include "SceneGraph/ISceneGraphManager.h"

struct ISceneCore
{
    virtual ~ISceneCore(){}

    /**
     * @brief 获取窗口管理类
     * @return
     */
    virtual ISceneGraphManager* GetSceneGraphManager()=0;

};

extern "C"
{
    /**
     * @brief 获取渲染场景核心库
     * @return
     */
    SCENECORESHARED_EXPORT ISceneCore* GetSceneCore();

    /**
     * @brief 检查电脑配置是否支持
     * @return
     */
    SCENECORESHARED_EXPORT bool         CheckPC();

    /**
     * @brief 设置exe运行路径
     * @param sPath
     */
    SCENECORESHARED_EXPORT void SetExePath(const string &sPath);

    /**
     * @brief 获取exe运行路径
     * @return
     */
    SCENECORESHARED_EXPORT const string& GetExePath();
}
#endif // ISCENCECORE_H
