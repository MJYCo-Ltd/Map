#include <algorithm>
#include <QThread>
#include <QCoreApplication>
#include <QLibrary>

#include <ISceneCore.h>
#include <Inner/IOsgSceneNode.h>
#include <SceneGraph/ISceneGroup.h>
#include <Plot/IPlot.h>
#include <Map/IMap.h>


#include "QtRender.h"
#include "QtViewPort.h"
#include "QtSceneGraph.h"
#include "Window/QtWindow.h"
#include "ResourceLod.h"


using namespace std;
QtSceneGraph::QtSceneGraph(SceneGraphType type):
    ISceneGraph(type),
    m_pRender(new QtRender),
    m_pThread(new QThread)
{
}

/// 析构函数
QtSceneGraph::~QtSceneGraph()
{
    Relase();
    /// 删除主窗口
    delete m_pMainWindow;

    /// 通知渲染器停止渲染
    QCoreApplication::postEvent(m_pRender,new QEvent(static_cast<QEvent::Type>(RENDER_STOP)));
    m_pRender->deleteLater();

    /// 删除所有的子窗口
    for(auto one : m_vOtherWindow)
    {
        delete one;
    }

    m_vOtherWindow.clear();

    ///关闭渲染线程
    if(m_pThread->isRunning())
    {
        m_pThread->quit();
        m_pThread->wait();
    }

    /// 释放资源
    delete m_pThread;
    delete m_pResourceLod;
}

/// 创建新的窗口
IWindow *QtSceneGraph::CreateNewWindow()
{
    if(m_bUnderQml)
    {
        return(nullptr);
    }

    auto newWindow = new QtWindow(this,m_pRender,m_pThread);
    m_vOtherWindow.push_back(newWindow);

    return(newWindow);
}

/// 删除窗口
bool QtSceneGraph::DeleteWindow(IWindow *& rWindow)
{
    ///只在子窗口中查找
    auto findResult = find(m_vOtherWindow.begin(),m_vOtherWindow.end(),rWindow);
    if(findResult != m_vOtherWindow.end())
    {
        delete dynamic_cast<QtWindow*>(rWindow);
        rWindow = nullptr;
        m_vOtherWindow.erase(findResult);
        return(true);
    }

    return(false);
}

/// 是否包含某窗口
bool QtSceneGraph::ContainWindow(IWindow *pWindow) const
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

bool QtSceneGraph::IsQuickItem(QQuickItem *pQuickItem) const
{
    return(pQuickItem == m_pQuickItem);
}

ILoadResouce *QtSceneGraph::ResouceLoader()
{
    return (m_pResourceLod);
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

/// 初始化场景
void QtSceneGraph::InitSceneGraph()
{
    /// 创建资源加载器
    m_pResourceLod = new CResourceLod;
    m_pResourceLod->InitPath(GetExePath());

    /// 创建主视口
    m_pMainWindow = new QtWindow(this,m_pRender,m_pThread);
    m_pMainWindow->InitWindow();
    auto pViewPoint = static_cast<QtViewPort*>(m_pMainWindow->GetMainViewPoint());

    /// 加载标绘接口
    LoadPlot();
    m_pPlot->RegisterPlotType();

    /// 如果是Map则加载earth文件
    if(SCENEGRAPH_USER != m_emType)
    {
        LoadMap();
        if(nullptr != m_pMap)
        {
            if(SCENEGRAPH_2D == m_emType)
            {
                pViewPoint->MapTypeChanged(MAP_2D);
            }
            else
            {
                pViewPoint->MapTypeChanged(MAP_3D);
            }

            m_pMap->AsOsgSceneNode()->Init();
            m_pMap->SubMessage(pViewPoint);

            m_pRoot = m_pMap;
        }
    }

    /// 如果没有地图数据则创建一个默认的节点
    if(nullptr == m_pRoot)
    {
        m_pRoot = m_pPlot->CreateSceneGroup(STANDARD_GROUP);
    }


    ///将场景树给主视口
    osgViewer::View* pView = pViewPoint->GetOsgView();
    auto pOsgNode = m_pRoot->AsOsgSceneNode()->GetOsgNode();
    pView->setSceneData(pOsgNode);

    ///将主视图关联到渲染器
    m_pRender->m_pOsgViewer->addView(pView);


    /// 开始渲染循环
    m_pRender->moveToThread(m_pThread);
}

/// 返回主窗口
IWindow *QtSceneGraph::GetMainWindow()
{
    return(m_pMainWindow);
}

/// 加载标绘接口
void QtSceneGraph::LoadPlot()
{
    typedef IPlot* (*CreatePlotFnc)(ISceneGraph*);
#ifdef Q_OS_WIN

#ifdef QT_NO_DEBUG
        QLibrary loadPlot("Plot.dll");
#else
        QLibrary loadPlot("Plotd.dll");
#endif

#else

#ifdef QT_NO_DEBUG
        QLibrary loadPlot("libPlot.so");
#else
        QLibrary loadPlot("libPlotd.so");
#endif

#endif
    if(loadPlot.load())
    {
        CreatePlotFnc pCreatePlot = reinterpret_cast<CreatePlotFnc>(loadPlot.resolve("CreatePlot"));
        if(nullptr != pCreatePlot)
        {
            m_pPlot = pCreatePlot(this);
        }
    }
}

/// 加载地图
void QtSceneGraph::LoadMap()
{
    typedef IMap* (*CreateMapFun)(MapType,ISceneGraph*);

#ifdef Q_OS_WIN

#ifdef QT_NO_DEBUG
        QLibrary loadMap("Map.dll");
#else
        QLibrary loadMap("Mapd.dll");
#endif

#else

#ifdef QT_NO_DEBUG
        QLibrary loadMap("libMap.so");
#else
        QLibrary loadMap("libMapd.so");
#endif

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
                break;
            default:
                m_pMap = pCreateMap(MAP_3D,this);
                break;
            }
        }
    }
}
