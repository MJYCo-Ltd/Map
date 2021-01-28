#ifndef INTERFACE_SCENE_CORE_H
#define INTERFACE_SCENE_CORE_H

#include <SceneCore_Global.h>
#include <SceneGraph/ISceneGraphManager.h>

#ifdef Q_OS_WIN
#ifdef QT_NO_DEBUG
static const char s_sSuffix[]=".dll";
#else
static const char s_sSuffix[]="d.dll";
#endif
#else
#ifdef QT_NO_DEBUG
static const char s_sSuffix[]=".so";
#else
static const char s_sSuffix[]="d.so";
#endif
#endif

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
    SCENECORESHARED_EXPORT void SetExePath(const std::string &sPath);

    /**
     * @brief 获取exe运行路径
     * @return
     */
    SCENECORESHARED_EXPORT const std::string& GetExePath();

    /**
     * @brief 获取资源所在路径
     * @return
     */
    SCENECORESHARED_EXPORT const std::string& GetDataPath();
}
#endif // ISCENCECORE_H
