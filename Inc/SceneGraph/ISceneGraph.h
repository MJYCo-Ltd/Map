#ifndef INTERFACE_SCENE_GRAPH_HEARDER_H
#define INTERFACE_SCENE_GRAPH_HEARDER_H
#include <Plot/IPlot.h>
class QQuickItem;
class ILoadResouce;
class IRender;
class IWindow;
class ISceneGroup;
class IMap;

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
    ISceneGraph(SceneGraphType emType):m_emType(emType){}
    /**
     * @brief 获取当前的显示类型
     * @return
     */
    virtual SceneGraphType GetType() const{return(m_emType);};

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
    virtual bool ContainWindow(IWindow*) const=0;

    /**
     * @brief 设置在qml下面运行
     * @attention 此函数非平台开发人员不要调用
     */
    virtual void SetQuickItem(QQuickItem*)=0;
    virtual bool IsQuickItem(QQuickItem*) const=0;
    virtual bool IsUnderQML() const{return(m_bUnderQml);}

    /**
     * @brief 获取地图
     * @return
     * @attention 只有SCENEGRAPH_3D和SCENEGRAPH_2D
     *            才能获取到有效的地图指针
     */
    virtual IMap* GetMap()=0;

    /**
     * @brief 获取场景根节点
     * @attention 如果加载的是地图，此处与返回的是IMap
     */
    virtual ISceneGroup* GetRoot(){return(nullptr);}

    /**
     * @brief 获取标绘接口
     * @return
     */
    virtual IPlot* GetPlot(){return(m_pPlot);}

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
protected:
    virtual ~ISceneGraph(){}

    /// 释放
    void Relase()
    {
        delete m_pPlot;
    }
protected:
    bool              m_bUnderQml=false;
    SceneGraphType    m_emType;
    IPlot*            m_pPlot=nullptr;
};

#endif
