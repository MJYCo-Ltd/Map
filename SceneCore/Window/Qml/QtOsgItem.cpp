#include "../QtFBOWindow.h"
#include "QtOsgRenderer.h"
#include <QTimer>

QtOsgItem::QtOsgItem(QQuickItem *parent):
    QQuickFramebufferObject(parent)
{
    setMirrorVertically(true);
    setAcceptedMouseButtons(Qt::AllButtons);
}

QtOsgItem::~QtOsgItem()
{
}

void QtOsgItem::setType(QtOsgItem::ItemType type)
{
    m_emType = type;
    m_emType = type;
    switch (m_emType)
    {
    case Item_2DMAP:
        m_pSceneGraph = GetSceneCore()->GetSceneGraphManager()->CreateSceneGraph(SCENEGRAPH_2D);
        break;
    case Item_User:
        m_pSceneGraph = GetSceneCore()->GetSceneGraphManager()->CreateSceneGraph(SCENEGRAPH_USER);
        break;
    default:
        m_pSceneGraph = GetSceneCore()->GetSceneGraphManager()->CreateSceneGraph(SCENEGRAPH_3D);
        break;
    }
    m_pSceneGraph->SetQuickItem(this);
}

QQuickFramebufferObject::Renderer *QtOsgItem::createRenderer() const
{
    return(new QtOsgRenderer(m_pSceneGraph));
}

/// 设置帧率
void QtOsgItem::setFrameRate(int nFrameRate)
{
    if(nFrameRate > 0 && nFrameRate != m_nFrameRate)
    {
        m_nFrameRate = nFrameRate;
    }
}

/// 已经准备好了
void QtOsgItem::Ready()
{
    m_pRenderer->InitSurface();
    QTimer::singleShot(1000/m_nFrameRate,this,&QtOsgItem::EvnetCall);
}

void QtOsgItem::EvnetCall()
{
    update();
    QTimer::singleShot(1000/m_nFrameRate,this,&QtOsgItem::EvnetCall);
}

/// 按键按下
void QtOsgItem::keyPressEvent(QKeyEvent *event)
{
    if(nullptr != m_pRenderer)
    {
        m_pRenderer->GetFBOWindow()->KeyPress(event);
    }
    event->accept();
}

/// 按键弹起
void QtOsgItem::keyReleaseEvent(QKeyEvent *event)
{
    if(nullptr != m_pRenderer)
    {
        m_pRenderer->GetFBOWindow()->KeyUp(event);
    }
    event->accept();
}

/// 鼠标按下消息
void QtOsgItem::mousePressEvent(QMouseEvent * event)
{
    if(nullptr != m_pRenderer)
    {
        m_pRenderer->GetFBOWindow()->MousePress(event);
    }
    event->accept();
}

/// 鼠标弹起消息
void QtOsgItem::mouseReleaseEvent(QMouseEvent *event)
{
    if(nullptr != m_pRenderer)
    {
        m_pRenderer->GetFBOWindow()->MouseUp(event);
    }
    event->accept();
}

void QtOsgItem::mouseMoveEvent(QMouseEvent *event)
{
    if(nullptr != m_pRenderer)
    {
        m_pRenderer->GetFBOWindow()->MouseMove(event);
    }
    event->accept();
}

///// 鼠标双击
void QtOsgItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(nullptr != m_pRenderer)
    {
        m_pRenderer->GetFBOWindow()->MouseDouble(event);
    }
    QQuickItem::mouseDoubleClickEvent(event);
}

/// 添加滚轮
void QtOsgItem::wheelEvent(QWheelEvent *event)
{
    if(nullptr != m_pRenderer)
    {
        m_pRenderer->GetFBOWindow()->WheelEvent(event);
    }
    QQuickItem::wheelEvent(event);
}

/// 触屏消息
void QtOsgItem::touchEvent(QTouchEvent *event)
{
    if(nullptr != m_pRenderer)
    {
        m_pRenderer->GetFBOWindow()->TouchEvent(event);
    }

    QQuickItem::touchEvent(event);
}
