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
     * @brief 移除渲染场景核心库
     */
    SCENECORESHARED_EXPORT void CloseSceneCore();

    /**
     * @brief 检查电脑配置是否支持
     * @param argv 为mian函数中第二个参数
     * @return
     */
    SCENECORESHARED_EXPORT bool CheckPC(char *argv[]);

    /**
     * @brief 将路径转成utf-8编码的字符串
     * @param sPath 指定路径
     * @return
     */
    SCENECORESHARED_EXPORT std::string Convert2Utf8(const std::string& sPath);

    /**
     * @brief 将路径统一成本地编码
     * @param sPath 指定路径
     * @return
     */
    SCENECORESHARED_EXPORT std::string Convert2Local(const std::string& sPath);

#ifdef NEED_VR
    /**
     * @brief 检查电脑是否连接VR设备
     * @return
     */
    SCENECORESHARED_EXPORT bool CheckVR();
#endif


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
