#ifndef INTERFACE_SCENE_GRAPH_HEARDER_H
#define INTERFACE_SCENE_GRAPH_HEARDER_H

#include <string>
#include "Window/IWindow.h"
#include "ISceneNode.h"
class QQuickItem;
class IMap;
class ILoadResouce;
class IRender;

using namespace std;

enum SceneGraphType
{
    SCENEGRAPH_3D,   /// 默认加载三维地图场景
    SCENEGRAPH_2D,   /// 默认加载二维地图场景
    SCENEGRAPH_USER  /// 用户自定义场景
};

/**
 * @brief 场景类
 */
class ISceneGraph
{
public:
    virtual ~ISceneGraph(){}

    /**
     * @brief 获取当前的显示类型
     * @return
     */
    virtual SceneGraphType GetType()=0;

    /**
     * @brief 获取主窗口
     * @return 主窗口指针
     */
    virtual IWindow* GetMainWindow()=0;

    /**
     * @brief 创建一个新的窗口
     * @return 窗口指针
     */
    virtual IWindow* CreateNewWindow()=0;

    /**
     * @brief 删除一个窗口
     * @return 是否删除成功
     */
    virtual bool DeleteWindow(IWindow*&)=0;

    /**
     * @brief 判断该场景是否包含某一个窗口
     * @return
     */
    virtual bool ContainWindow(IWindow*)=0;

    /**
     * @brief 设置在qml下面运行
     * @attention 此函数非平台开发人员不要调用
     */
    virtual void SetQuickItem(QQuickItem*)=0;
    virtual bool IsQuickItem(QQuickItem*)=0;
    virtual bool IsUnderQML()=0;

    /**
     * @brief 获取地图
     * @return
     * @attention 只有SCENEGRAPH_3D和SCENEGRAPH_2D
     *            才能获取到有效的地图指针
     */
    virtual IMap* GetMap()=0;

    /**
     * @brief 获取场景根节点
     * @return
     */
    virtual ISceneNode* GetRoot()=0;

    /**
     * @brief 加载模型
     * @return 模型节点
     */
    virtual ISceneNode* LoadNode(const string&)=0;

    /**
     * @brief 资源加载器
     * @return
     */
    virtual ILoadResouce* ResouceLoader()=0;

    /**
     * @brief 获取渲染器
     * @return
     */
    virtual IRender* SceneGraphRender()=0;

    /**
     * @brief 渲染
     */
    virtual void Run()=0;
};

#endif
