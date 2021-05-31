#ifndef QTSCENEGRAPH_H
#define QTSCENEGRAPH_H
#include <list>
#include "SceneGraph/ISceneGraph.h"

class IOsgSceneGroup;
class QtWindow;
class QThread;
class QtRender;
class CResourceLod;
class QtViewPort;

class QtSceneGraph : public ISceneGraph
{
public:
    explicit QtSceneGraph(SceneGraphType type);
    ~QtSceneGraph();

    /**
     * @brief 初始化场景
     */
    void InitSceneGraph();

    /**
     * @brief 获取主窗口
     * @return 主窗口指针
     */
    IWindow* GetMainWindow();

    /**
     * @brief 创建一个新的窗口
     * @return 窗口指针
     */
    IWindow* CreateNewWindow();

    /**
     * @brief 删除一个窗口
     * @return 是否删除成功
     */
    bool DeleteWindow(IWindow*&rWindow);

    /**
     * @brief 是否包含某窗口
     * @return
     */
    bool ContainWindow(IWindow* pWindow) const;

    /**
     * @brief 设置在qml下面运行
     */
    virtual void SetQuickItem(QQuickItem* pQuickItem);
    virtual bool IsQuickItem(QQuickItem* pQuickItem) const;

    IMap* GetMap(){return(m_pMap);}
    /**
     * @brief 获取根节点
     * @return
     */
    ISceneGroup* GetRoot(){return(m_pRoot);}

    /**
     * @brief 资源加载器
     * @return
     */
    ILoadResouce* ResouceLoader();

    /**
     * @brief 获取渲染器
     * @return
     */
    IRender* SceneGraphRender();

    /**
     * @brief 渲染
     */
    void Run();

    /**
     * @brief 获取渲染线程
     */
    QThread* GetRenderThread(){return(m_pThread);}

protected:

    /**
     * @brief 加载标绘接口
     */
    void LoadPlot();

    /**
     * @brief 加载工具
     */
    void LoadTool();

    /**
     * @brief 加载地图
     */
    void LoadMap();


protected:
    ISceneGroup*      m_pRoot{};
    IMap*             m_pMap{};
    QtWindow*         m_pMainWindow{};
    QQuickItem*       m_pQuickItem{};
    QtRender*         m_pRender{};
    QThread*          m_pThread{};
    CResourceLod*     m_pResourceLod{};
    std::list<QtWindow*> m_vOtherWindow;
};

#endif // QTSCENEGRAPH_H
