#include "../QtFBOWindow.h"
#include "QtOsgRenderer.h"
#include <QTimer>

QtOsgItem::QtOsgItem(QQuickItem *parent):
    QQuickFramebufferObject(parent)
{
    setMirrorVertically(true);
    setTextureFollowsItemSize(true);
    /// 有节点
    //    setFlag(ItemHasContents, true);
    //    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);
    //    setFlag(ItemAcceptsInputMethod,true);
    //    grabMouse();
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
    case Item_2DMAp:
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

/// 已经准备好了
void QtOsgItem::Ready()
{
    m_pRenderer->InitSurface();
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
