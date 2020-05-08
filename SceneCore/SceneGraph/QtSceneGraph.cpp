#include <algorithm>
#include <QThread>
#include <QCoreApplication>
#include <QLibrary>
#include <osgDB/ReadFile>

#include "QtRender.h"
#include "QtViewPoint.h"
#include "QtSceneGraph.h"
#include "Window/QtWindow.h"
#include "ResourceLod.h"
#include "Map/IMap.h"
#include "ISceneCore.h"

using namespace std;
QtSceneGraph::QtSceneGraph(SceneGraphType type):
    m_bUnderQml(false),
    m_emType(type),
    m_pRender(new QtRender),
    m_pThread(new QThread)
{
    InitSceneGraph();
}

/// 析构函数
QtSceneGraph::~QtSceneGraph()
{
    delete m_pRoot;
    delete m_pMap;

    /// 删除主窗口
    delete m_pMainWindow;
    QCoreApplication::postEvent(m_pRender,new QEvent(static_cast<QEvent::Type>(RENDER_STOP)));
    m_pRender->deleteLater();

    /// 删除所有的从窗口
    for(auto one : m_vOtherWindow)
    {
        delete one;
    }

    m_vOtherWindow.clear();

    if(m_pThread->isRunning())
    {
        m_pThread->quit();
        m_pThread->wait();
    }

    delete m_pThread;

    delete m_pResourceLod;
}

/// 切换类型
bool QtSceneGraph::ChangeType(SceneGraphType type)
{
    if(SCENEGRAPH_USER == m_emType || SCENEGRAPH_USER == type
            || type == m_emType)
    {
        return(false);
    }

    /// 设置状态
    m_emType = type;
    return(true);
}

/// 获取场景类型
SceneGraphType QtSceneGraph::GetType()
{
    return(m_emType);
}

/// 获取主窗口
IWindow *QtSceneGraph::GetMainWindow()
{
    return(m_pMainWindow);
}

/// 创建新的窗口
IWindow *QtSceneGraph::CreateNewWindow()
{
    if(m_bUnderQml)
    {
        return(nullptr);
    }

    auto newWindow = new QtWindow(m_pRender,m_pThread);
    m_vOtherWindow.push_back(newWindow);

    return(newWindow);
}

/// 删除窗口
bool QtSceneGraph::DeleteWindow(IWindow *& rWindow)
{
    auto findResult = find(m_vOtherWindow.begin(),m_vOtherWindow.end(),rWindow);
    if(findResult != m_vOtherWindow.end())
    {
        delete rWindow;
        rWindow = nullptr;
        m_vOtherWindow.erase(findResult);
        return(true);
    }

    return(false);
}

/// 是否包含某窗口
bool QtSceneGraph::ContainWindow(IWindow *pWindow)
{
    if(pWindow == m_pMainWindow)
    {
        return(true);
    }


    for(auto one : m_vOtherWindow)
    {
        if(pWindow == one)
        {
            return(true);
        }
    }

    return(false);
}

/// 设置在qml下运行
void QtSceneGraph::SetQuickItem(QQuickItem* pQuickItem)
{
    m_pQuickItem = pQuickItem;
    m_bUnderQml = m_pQuickItem != nullptr;
}

bool QtSceneGraph::IsQuickItem(QQuickItem *pQuickItem)
{
    return(pQuickItem == m_pQuickItem);
}

bool QtSceneGraph::IsUnderQML()
{
    return(m_bUnderQml);
}

/// 获取地图
IMap *QtSceneGraph::GetMap()
{
    return (m_pMap);
}

ISceneNode *QtSceneGraph::GetRoot()
{
    return(m_pRoot);
}

/// 加载节点
ISceneNode *QtSceneGraph::LoadNode(const string &strPath)
{
    auto node = m_pResourceLod->LoadNode(strPath,false);
    if(nullptr != node)
    {
        auto result = new QtSceneNode<ISceneNode>(this);
        result->InitSceneNode();
        result->GetOsgNode()->asGroup()->addChild(node);
        return(result);
    }

    return(nullptr);
}

/// 获取资源加载器
ILoadResouce *QtSceneGraph::ResouceLoader()
{
    return(m_pResourceLod);
}

IRender *QtSceneGraph::SceneGraphRender()
{
    return(m_pRender);
}

/// 渲染
void QtSceneGraph::Run()
{
    m_pThread->start();
}

/// 获取线程
QThread* QtSceneGraph::GetRenderThread()
{
    return(m_pThread);
}

/// 初始化场景
void QtSceneGraph::InitSceneGraph()
{
    m_pResourceLod = new CResourceLod;
    m_pRoot = new QtSceneNode<ISceneNode>(this);
    /// 初始化主视图
    m_pMainWindow = new QtWindow(m_pRender,m_pThread,true);
    auto pViewPoint = static_cast<QtViewPoint*>(m_pMainWindow->GetMainViewPoint());

    /// 初始化路径
    m_pRoot->InitSceneNode();
    m_pResourceLod->InitPath(GetExePath());
    /// 如果是地图
    if(SCENEGRAPH_USER != m_emType)
    {
        LoadMap(pViewPoint);
    }
    else
    {
    }


    osgViewer::View* pView = pViewPoint->GetOsgView();
    pView->setSceneData(m_pRoot->GetOsgNode());
    m_pRender->m_pOsgViewer->addView(pView);


    m_pRender->moveToThread(m_pThread);
}

void QtSceneGraph::LoadMap(QtViewPoint* pViewPoint)
{
    typedef IMap* (*CreateMapFun)(MapType,ISceneGraph*);
#ifdef _DEBUG
    QLibrary loadMap("Mapd.dll");
#else
    QLibrary loadMap("Map.dll");
#endif
    if(loadMap.load())
    {
        CreateMapFun pCreateMap = reinterpret_cast<CreateMapFun>(loadMap.resolve("CreateMap"));
        if(nullptr != pCreateMap)
        {
            switch (m_emType)
            {
            case SCENEGRAPH_2D:
                m_pMap = pCreateMap(MAP_2D,this);
                pViewPoint->MapTypeChanged(MAP_2D);
                break;
            case SCENEGRAPH_3D:
                m_pMap = pCreateMap(MAP_3D,this);
                pViewPoint->MapTypeChanged(MAP_3D);
                break;
            }

            m_pMap->InitSceneNode();
            m_pRoot->AddSceneNode(m_pMap);
            m_pMap->SubMessage(pViewPoint);
        }
    }
}
