#ifndef QTSCENEGRAPH_H
#define QTSCENEGRAPH_H
#include <list>
#include "SceneGraph/ISceneGraph.h"
using namespace std;

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
     * @brief 加载节点
     * @param strPath  节点路径
     * @return
     */
    ISceneModel* LoadNode(const string& strPath,bool);

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
     * @brief 加载地图
     */
    void LoadMap();


protected:
    ISceneGroup*      m_pRoot=nullptr;
    IMap*             m_pMap=nullptr;
    QtWindow*         m_pMainWindow;
    QQuickItem*       m_pQuickItem=nullptr;
    QtRender*         m_pRender;
    QThread*          m_pThread;
    CResourceLod*     m_pResourceLod;
    list<QtWindow*> m_vOtherWindow;
};

#endif // QTSCENEGRAPH_H
