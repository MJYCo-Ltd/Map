#ifndef QTSCENEGRAPH_H
#define QTSCENEGRAPH_H
#include <list>
#include "SceneGraph/ISceneGraph.h"
#include "Inner/QtOsgSceneNode.h"
using namespace std;

class IOsgSceneNode;
class QtWindow;
class QThread;
class QtRender;
class CResourceLod;
class QtViewPoint;

class QtSceneGraph : public ISceneGraph
{
public:
    explicit QtSceneGraph(SceneGraphType type);
    ~QtSceneGraph();

    /**
     * @brief 切换类型
     * @param type
     * @return
     */
    bool ChangeType(SceneGraphType type);

    /**
     * @brief 获取场景类型
     * @return 场景类型
     */
    SceneGraphType GetType();

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
    bool ContainWindow(IWindow* pWindow);

    /**
     * @brief 设置在qml下面运行
     */
    virtual void SetQuickItem(QQuickItem* pQuickItem);
    virtual bool IsQuickItem(QQuickItem* pQuickItem);
    virtual bool IsUnderQML();

    /**
     * @brief 获取地图
     * @return
     */
    IMap* GetMap();

    /**
     * @brief 获取根节点
     * @return
     */
    ISceneNode* GetRoot();

    /**
     * @brief 加载节点
     * @param strPath  节点路径
     * @return
     */
    ISceneNode* LoadNode(const string& strPath);

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
    QThread* GetRenderThread();

protected:

    /**
     * @brief 初始化场景
     */
    void InitSceneGraph();

    /**
     * @brief 加载地图
     */
    void LoadMap(QtViewPoint *pViewPoint);

protected:
    IOsgSceneNode*    m_pRoot;
    bool              m_bUnderQml;
    SceneGraphType    m_emType;
    QtWindow*         m_pMainWindow;
    QQuickItem*       m_pQuickItem=nullptr;
    QtRender*         m_pRender;
    QThread*          m_pThread;
    CResourceLod*     m_pResourceLod;
    IMap*             m_pMap=nullptr;
    list<QtWindow*> m_vOtherWindow;
};

#endif // QTSCENEGRAPH_H
