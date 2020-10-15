#include <QQuickWindow>
#include <QOpenGLContext>
#include <QDateTime>

#include "../QtFBOWindow.h"
#include "../QtEventMap.h"
#include "../QtWindow.h"
#include "SceneGraph/QtSceneGraph.h"
#include "SceneGraph/QtRender.h"
#include "SGSimpleTextureNode.h"
#include "Qml/QtOsgItem.h"
#include "ISceneCore.h"

#include <QTimer>

QtOsgItem::QtOsgItem():
    m_unTextureID(0),
    m_pShow(nullptr)
{
    m_pSceneGraph = GetSceneCore()->GetSceneGraphManager()->CreateSceneGraph(SCENEGRAPH_3D,"");
    m_pSceneGraph->SetQuickItem(this);

    m_pFBOWindow = static_cast<QtWindow*>(m_pSceneGraph->GetMainWindow())->GetOsgWindow();

    /// 有节点
    setFlag(ItemHasContents, true);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setFlag(ItemAcceptsInputMethod,true);
    grabMouse();

    QtRender* pRender = static_cast<QtRender*>(static_cast<QtSceneGraph*>(m_pSceneGraph)->SceneGraphRender());

    connect(pRender,&QtRender::RenderAFrame,this,&QtOsgItem::UpdateTexture);
}

QtOsgItem::~QtOsgItem()
{
    /// 删除整个场景
    GetSceneCore()->GetSceneGraphManager()->DeleteSceneGraph(m_pSceneGraph);
}

ISceneGraph *QtOsgItem::GetSceneGraph()
{
    return(m_pSceneGraph);
}

QtFBOWindow *QtOsgItem::GetFBOWindow()
{
    return(m_pFBOWindow);
}

/// 已经准备好了
void QtOsgItem::Ready()
{
    /// 初始化绘制的窗口
    /// 窗口的初始化只能在 gui thread中进行
    m_pFBOWindow->InitSurface(static_cast<QtSceneGraph*>(m_pSceneGraph)->GetRenderThread());
    m_pSceneGraph->Run();

    QtRender* pRender = static_cast<QtRender*>(static_cast<QtSceneGraph*>(m_pSceneGraph)->SceneGraphRender());

    /// 重置窗口大小
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
    QCoreApplication::postEvent(pRender,new RenderResize(this,this->size().toSize()));
#else
    QCoreApplication::postEvent(pRender,new RenderResize(this,QSize(this->width(),this->height())));
#endif
    QCoreApplication::postEvent(pRender,new QEvent(static_cast<QEvent::Type>(RENDER_START)));
    //m_nTimerID = startTimer(16);
}

/// 按键按下
void QtOsgItem::keyPressEvent(QKeyEvent *event)
{
    m_pFBOWindow->KeyPress(event);
    event->accept();
}

/// 按键弹起
void QtOsgItem::keyReleaseEvent(QKeyEvent *event)
{
    m_pFBOWindow->KeyUp(event);
    event->accept();
}

/// 鼠标按下消息
void QtOsgItem::mousePressEvent(QMouseEvent * event)
{
    m_pFBOWindow->MousePress(event);
    event->accept();
}

/// 鼠标弹起消息
void QtOsgItem::mouseReleaseEvent(QMouseEvent *event)
{
    m_pFBOWindow->MouseUp(event);
    event->accept();
}

void QtOsgItem::mouseMoveEvent(QMouseEvent *event)
{
    m_pFBOWindow->MouseMove(event);
    event->accept();
}

///// 鼠标双击
void QtOsgItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    m_pFBOWindow->MouseDouble(event);
    QQuickItem::mouseDoubleClickEvent(event);
}

/// 添加滚轮
void QtOsgItem::wheelEvent(QWheelEvent *event)
{
    m_pFBOWindow->WheelEvent(event);
    QQuickItem::wheelEvent(event);
}

/// 触屏消息
void QtOsgItem::touchEvent(QTouchEvent *event)
{
    m_pFBOWindow->TouchEvent(event);

    QQuickItem::touchEvent(event);
}

void QtOsgItem::timerEvent(QTimerEvent *event)
{
    if(m_nTimerID == event->timerId())
    {
        qDebug()<<"Timer";
        update();
    }
}

/// 窗口大小更改时，创建view
void QtOsgItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QtRender* pRender = static_cast<QtRender*>(static_cast<QtSceneGraph*>(m_pSceneGraph)->SceneGraphRender());
    QCoreApplication::postEvent(pRender,new RenderResize(this,newGeometry.size().toSize()));
    QQuickItem::geometryChanged(newGeometry,oldGeometry);
    if(nullptr != m_pShow)
    {
        m_pShow->setRect(boundingRect());
    }
}

/// 获取渲染节点添加到渲染树上
/// 本函数 由QSGRenderThread 调用
/// 在这个线程中 不能发送信号
/// 且节点添加上，就不再更改
/// 此时GUI主线程被阻塞，因此可以调用QuickItem中的变量
QSGNode *QtOsgItem::updatePaintNode(QSGNode * oldNode, QQuickItem::UpdatePaintNodeData *)
{
    m_pShow = static_cast<QtOSGSimpleTextureNode*>(oldNode);


    if(nullptr == m_pShow)
    {
        /// 初始化设备上下文
        window()->openglContext()->doneCurrent();
        m_pFBOWindow->InitContext(window()->openglContext());
        window()->openglContext()->makeCurrent(window());

        /// 准备创建窗口
        QMetaObject::invokeMethod(this,"Ready");

        /// 创建qml显示节点
        m_pShow = new QtOSGSimpleTextureNode(window());

    }

    m_pShow->RebuildTexture();

    if(m_bUpdate)
    {
        QtRender* pRender = static_cast<QtRender*>(static_cast<QtSceneGraph*>(m_pSceneGraph)->SceneGraphRender());
        QCoreApplication::postEvent(pRender,new QEvent(static_cast<QEvent::Type>(RENDER_START)));
        m_bUpdate = false;
    }

    return(m_pShow);
}

/// 获取新的纹理
void QtOsgItem::UpdateTexture()
{
    if(nullptr != m_pShow)
    {
        m_pShow->UpdateTexture(m_pFBOWindow->GetFBOTextureID());
        m_pShow->UpdateSize(m_pFBOWindow->GetFBOSize());
        m_bUpdate = true;
        update();
    }
}
