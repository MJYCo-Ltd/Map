#include <QSize>
#include <QThread>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions>

#include <QThread>

#include <SceneGraph/IWindow.h>
#include <SceneGraph/ViewType.h>
#include "QtEventMap.h"
#include "QtFBOWindow.h"

inline MouseButtonMask ChangeMouseType(QMouseEvent* event)
{
    MouseButtonMask button;
    switch (event->button())
    {
    case Qt::LeftButton: button = LEFT_MOUSE_BUTTON; break;
    case Qt::MiddleButton: button = MIDDLE_MOUSE_BUTTON; break;
    case Qt::RightButton: button = RIGHT_MOUSE_BUTTON; break;
    default: button = UNKNOWN_BUTTON; break;
    }

    return(button);
}

QtFBOWindow::QtFBOWindow(list<IWindowMessageObserver *> *pAllObserver)
    :osgViewer::GraphicsWindowEmbedded(0,0,C_WINDOW_WIDTH,C_WINDOW_HEIGHT)
    ,m_bInit(false)
    ,m_nInitFBO(0)
    ,m_nTextureID(0)
    ,m_stWinSize(C_WINDOW_WIDTH,C_WINDOW_HEIGHT)
    ,m_pOffScreenSurface(new QOffscreenSurface)
    ,m_pOpenglContext(new QOpenGLContext)
    ,m_pRenderFbo(nullptr)
    ,m_pDisplayFbo(nullptr)
    ,m_pAllOserver(pAllObserver)
{
    getEventQueue()->windowResize(0, 0, m_stWinSize.width() , m_stWinSize.height());
}

QtFBOWindow::~QtFBOWindow()
{
    if(m_bInit)
    {
        /// 关联设备上下文
        if(0 != m_nInitFBO)
        {
            m_pOpenglContext->makeCurrent(m_pOffScreenSurface);
            delete m_pRenderFbo;
            delete m_pDisplayFbo;
            m_pOpenglContext->doneCurrent();
        }

        /// 删除窗口
        delete m_pOpenglContext;
        delete m_pOffScreenSurface;
    }
}

/// 重设窗口大小
void QtFBOWindow::ReSize(const QSize &rWinSize)
{
    if(m_stWinSize == rWinSize)
    {
        return;
    }
    else
    {
        m_stWinSize = rWinSize;

        /// 重置窗口大小
        getEventQueue()->windowResize(0,0,m_stWinSize.width(),m_stWinSize.height());
        resized(0,0,m_stWinSize.width(),m_stWinSize.height());
        m_nInitFBO = 0;
    }
}

/// 初始化FBO
bool QtFBOWindow::InitContext(QOpenGLContext *pToShareContext)
{
    /// 防止重复初始化
    if(m_bInit)
    {
        return(m_bInit);
    }

    /// 设置opengl设备上下文格式
    m_pOpenglContext->setFormat(pToShareContext->format());
    m_pOpenglContext->setShareContext(pToShareContext);


    m_bInit = m_pOpenglContext->create();

    return(m_bInit);
}

void QtFBOWindow::InitSurface(QThread *pThread)
{
    /// 设置渲染窗口格式
    m_pOffScreenSurface->setFormat(m_pOpenglContext->format());
    m_pOffScreenSurface->create();
    m_pOpenglContext->moveToThread(pThread);
}


/// 关联设备上下文
bool QtFBOWindow::makeCurrentImplementation()
{
    if(2 != m_nInitFBO)
    {
        InitFBO();
    }

    /// 设置默认的FBO
    setDefaultFboId(m_pRenderFbo->handle());
    m_pRenderFbo->bind();

    return(true);
}

/// 释放设备上下文
bool QtFBOWindow::releaseContextImplementation()
{
    /// 调用默认的设备
    QOpenGLFramebufferObject::bindDefault();

    return(true);
}

/// 交换帧缓存区
void QtFBOWindow::swapBuffersImplementation()
{
    m_pOpenglContext->functions()->glFlush();
    qSwap(m_pRenderFbo, m_pDisplayFbo);
    m_nTextureID = m_pDisplayFbo->texture();
    m_stTextureSize = m_pDisplayFbo->size();
}


/// 初始化 帧缓存对象
void QtFBOWindow::InitFBO()
{
    if(2 == m_nInitFBO)
    {
        return;
    }

    if(nullptr != m_pRenderFbo)
    {
        delete m_pRenderFbo;
        m_pRenderFbo = nullptr;
    }

    m_pOpenglContext->makeCurrent(m_pOffScreenSurface);

    /// 设置FBO的格式
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    m_pRenderFbo = new QOpenGLFramebufferObject(m_stWinSize, format);

    ++m_nInitFBO;
}

/// 键盘消息
void QtFBOWindow::keyboardModifiers(QInputEvent *event)
{
    int modkey = event->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier);
    int mask = 0;
    if (modkey & Qt::ShiftModifier) mask |= osgGA::GUIEventAdapter::MODKEY_SHIFT;
    if (modkey & Qt::ControlModifier) mask |= osgGA::GUIEventAdapter::MODKEY_CTRL;
    if (modkey & Qt::AltModifier) mask |= osgGA::GUIEventAdapter::MODKEY_ALT;

    getEventQueue()->getCurrentEventState()->setModKeyMask(mask);
}

void QtFBOWindow::KeyPress(QKeyEvent *event)
{
    for(auto one : *m_pAllOserver)
    {
        one->KeyDown(QtEventdMap::GetInstance()->ChangeKeyEvent(event));
    }

    keyboardModifiers(event);
    getEventQueue()->keyPress(QtEventdMap::GetInstance()->ChangeKeyEvent(event));
}

void QtFBOWindow::KeyUp(QKeyEvent *event)
{
    for(auto one : *m_pAllOserver)
    {
        one->KeyUp(QtEventdMap::GetInstance()->ChangeKeyEvent(event));
    }

    keyboardModifiers(event);
    getEventQueue()->keyRelease(QtEventdMap::GetInstance()->ChangeKeyEvent(event));
}

void QtFBOWindow::MouseDouble(QMouseEvent *event)
{
    for(auto one : *m_pAllOserver)
    {
        one->MouseDblClick(ChangeMouseType(event),event->x(),event->y());
    }

    keyboardModifiers(event);
    getEventQueue()->mouseDoubleButtonPress(event->x(),event->y(),QtEventdMap::GetInstance()->ChangeMouseEvent(event));
}

/// 鼠标按下
void QtFBOWindow::MousePress(QMouseEvent *event)
{
    for(auto one : *m_pAllOserver)
    {
        one->MouseDown(ChangeMouseType(event),event->x(),event->y());
    }

    keyboardModifiers(event);
    getEventQueue()->mouseButtonPress(event->x(), event->y(), QtEventdMap::GetInstance()->ChangeMouseEvent(event));
}

/// 鼠标弹起
void QtFBOWindow::MouseUp(QMouseEvent *event)
{
    for(auto one : *m_pAllOserver)
    {
        one->MouseUp(ChangeMouseType(event),event->x(),event->y());
    }

    keyboardModifiers(event);
    getEventQueue()->mouseButtonRelease(event->x(), event->y(), QtEventdMap::GetInstance()->ChangeMouseEvent(event));
}

/// 鼠标移动
void QtFBOWindow::MouseMove(QMouseEvent *event)
{
    for(auto one : *m_pAllOserver)
    {
        one->MouseMove(ChangeMouseType(event),event->x(),event->y());
    }

    keyboardModifiers(event);
    getEventQueue()->mouseMotion(event->x(), event->y());
}

void QtFBOWindow::WheelEvent(QWheelEvent *event)
{
    keyboardModifiers(event);
    getEventQueue()->mouseScroll(QtEventdMap::GetInstance()->ChangeWheelEvent(event));
}

/// 触屏消息
void QtFBOWindow::TouchEvent(QTouchEvent *event)
{
    switch (event->touchPointStates())
    {
    case Qt::TouchPointPressed:
    {
        QList<QTouchEvent::TouchPoint> touchPoints = event->touchPoints();
        unsigned int id = 0;
        osg::ref_ptr<osgGA::GUIEventAdapter> osgEvent(nullptr);
        foreach(const QTouchEvent::TouchPoint& touchPoint, touchPoints)
        {
            if (!osgEvent.valid())
            {
                keyboardModifiers(event);
                osgEvent = getEventQueue()->touchBegan(id, osgGA::GUIEventAdapter::TOUCH_BEGAN
                                                       , static_cast<float>(touchPoint.pos().x()), static_cast<float>(touchPoint.pos().y()));
            }
            else
            {
                osgEvent->addTouchPoint(id, osgGA::GUIEventAdapter::TOUCH_BEGAN,
                                        static_cast<float>(touchPoint.pos().x()), static_cast<float>(touchPoint.pos().y()));
            }
            id++;
        }
    }
        break;
    case Qt::TouchPointReleased:
    {
        QList<QTouchEvent::TouchPoint> touchPoints =event->touchPoints();
        unsigned int id = 0;
        osg::ref_ptr<osgGA::GUIEventAdapter> osgEvent(nullptr);
        foreach(const QTouchEvent::TouchPoint& touchPoint, touchPoints)
        {
            unsigned int tap_count = static_cast<unsigned int>(touchPoints.size());
            if (!osgEvent.valid())
            {
                keyboardModifiers(event);
                osgEvent = getEventQueue()->touchEnded(id, osgGA::GUIEventAdapter::TOUCH_ENDED,
                                                       static_cast<float>(touchPoint.pos().x()), static_cast<float>(touchPoint.pos().y()), tap_count);
            }
            else
            {
                osgEvent->addTouchPoint(id, osgGA::GUIEventAdapter::TOUCH_ENDED,
                                        static_cast<float>(touchPoint.pos().x()), static_cast<float>(touchPoint.pos().y()));
            }
            ++id;
        }
    }
        break;
    case Qt::TouchPointMoved:
    {
        QList<QTouchEvent::TouchPoint> touchPoints = event->touchPoints();
        unsigned int id = 0;
        osg::ref_ptr<osgGA::GUIEventAdapter> osgEvent(nullptr);
        foreach(const QTouchEvent::TouchPoint& touchPoint, touchPoints)
        {

            if (!osgEvent.valid())
            {
                keyboardModifiers(event);
                osgEvent = getEventQueue()->touchMoved(id, osgGA::GUIEventAdapter::TOUCH_MOVED,
                                                       static_cast<float>(touchPoint.pos().x()), static_cast<float>(touchPoint.pos().y()));
            }
            else
            {
                osgEvent->addTouchPoint(id, osgGA::GUIEventAdapter::TOUCH_MOVED,
                                        static_cast<float>(touchPoint.pos().x()), static_cast<float>(touchPoint.pos().y()));
            }
            ++id;
        }
    }
        break;
    case Qt::TouchPointStationary:
    {
        QList<QTouchEvent::TouchPoint> touchPoints = event->touchPoints();
        unsigned int id = 0;
        osg::ref_ptr<osgGA::GUIEventAdapter> osgEvent(nullptr);
        foreach(const QTouchEvent::TouchPoint& touchPoint, touchPoints)
        {

            if (!osgEvent.valid())
            {
                keyboardModifiers(event);
                osgEvent = getEventQueue()->touchMoved(id, osgGA::GUIEventAdapter::TOUCH_STATIONERY,
                                                       static_cast<float>(touchPoint.pos().x()), static_cast<float>(touchPoint.pos().y()));
            }
            else
            {
                osgEvent->addTouchPoint(id, osgGA::GUIEventAdapter::TOUCH_STATIONERY,
                                        static_cast<float>(touchPoint.pos().x()), static_cast<float>(touchPoint.pos().y()));
            }
            ++id;
        }
    }
        break;
    }
}
