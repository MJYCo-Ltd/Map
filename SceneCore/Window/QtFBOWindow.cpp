#include <QSize>
#include <QThread>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QHoverEvent>
#include <QDropEvent>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions>

#include <QThread>

#include <SceneGraph/IWindow.h>
#include <SceneGraph/ViewType.h>
#include "QtEventMap.h"
#include "QtFBOWindow.h"
#include <QDebug>
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

QtFBOWindow::QtFBOWindow(std::list<IWindowMessageObserver *> *pAllObserver)
    :osgViewer::GraphicsWindowEmbedded(0,0,C_WINDOW_WIDTH,C_WINDOW_HEIGHT)
    ,m_stWinSize(C_WINDOW_WIDTH,C_WINDOW_HEIGHT)
    ,m_pOffScreenSurface(new QOffscreenSurface)
    ,m_pOpenglContext(new QOpenGLContext)
    ,m_pAllOserver(pAllObserver)
{
    getEventQueue()->windowResize(0, 0, m_stWinSize.width() , m_stWinSize.height());
}

QtFBOWindow::~QtFBOWindow()
{
    if(m_bInit)
    {
        /// 关联设备上下文
        if(m_bInitFBO)
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
        m_bInitFBO = false;
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

///
uint QtFBOWindow::GetFBOTextureID()
{
    m_bCanSwap=true;
    return(m_unTextureID);
}


/// 关联设备上下文
bool QtFBOWindow::makeCurrentImplementation()
{
    m_pOpenglContext->makeCurrent(m_pOffScreenSurface);
    if(!m_bInitFBO)
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
    m_pOpenglContext->doneCurrent();
    return(true);
}

#ifdef NEED_VR
#include <openvr/openvr.h>
#endif
/// 交换帧缓存区
void QtFBOWindow::swapBuffersImplementation()
{
    m_pOpenglContext->functions()->glFinish();

    m_unTextureID = m_pRenderFbo->texture();
#ifdef NEED_VR
    if(vr::VRCompositor())
    {
        vr::VRCompositor()->SetTrackingSpace(vr::TrackingUniverseSeated);

        vr::TrackedDevicePose_t poses[vr::k_unMaxTrackedDeviceCount];
        for (int i = 0; i < vr::k_unMaxTrackedDeviceCount; ++i) poses[i].bPoseIsValid = false;
        vr::VRCompositor()->WaitGetPoses(poses, vr::k_unMaxTrackedDeviceCount, NULL, 0);

        vr::Texture_t leftEyeTexture = {(void*)m_unTextureID, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
        vr::Texture_t rightEyeTexture = {(void*)m_unTextureID, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
        vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);
        vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);
    }
#endif

    if(m_bCanSwap)
    {
        qSwap(m_pRenderFbo, m_pDisplayFbo);
        m_bCanSwap=false;
    }
}


/// 初始化 帧缓存对象
void QtFBOWindow::InitFBO()
{
    if(m_bInitFBO)
    {
        return;
    }

    if(nullptr != m_pRenderFbo)
    {
        delete m_pRenderFbo;
        m_pRenderFbo = nullptr;
    }
    if(nullptr != m_pDisplayFbo)
    {
        delete m_pDisplayFbo;
        m_pDisplayFbo=nullptr;
    }

    /// 设置FBO的格式
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    m_pRenderFbo = new QOpenGLFramebufferObject(m_stWinSize, format);
    m_pDisplayFbo = new QOpenGLFramebufferObject(m_stWinSize,format);
    m_stTextureSize=m_stWinSize;

    m_bInitFBO=true;
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
    auto iter = m_pAllOserver->begin();
    auto tmpIter = iter;

    int nKey = QtEventdMap::GetInstance()->ChangeKeyEvent(event);
    for(;iter != m_pAllOserver->end(); )
    {
        ++tmpIter;
        (*iter)->KeyDown(nKey);
        iter = tmpIter;
    }

    keyboardModifiers(event);
    getEventQueue()->keyPress(nKey);
}

void QtFBOWindow::KeyUp(QKeyEvent *event)
{
    auto iter = m_pAllOserver->begin();
    auto tmpIter = iter;

    int nKey = QtEventdMap::GetInstance()->ChangeKeyEvent(event);
    for(;iter != m_pAllOserver->end(); )
    {
        ++tmpIter;
        (*iter)->KeyUp(nKey);
        iter = tmpIter;
    }

    keyboardModifiers(event);
    getEventQueue()->keyRelease(nKey);
}

void QtFBOWindow::MouseDouble(QMouseEvent *event, qreal rScal)
{
    m_rMouseX = event->x()*rScal;
    m_rMouseY = event->y()*rScal;

    auto iter = m_pAllOserver->begin();
    auto tmpIter = iter;
    MouseButtonMask mask = ChangeMouseType(event);
    for(;iter != m_pAllOserver->end(); )
    {
        ++tmpIter;
        (*iter)->MouseDblClick(mask,m_rMouseX,m_rMouseY);
        iter = tmpIter;
    }

    keyboardModifiers(event);
    getEventQueue()->mouseDoubleButtonPress(m_rMouseX,m_rMouseY,QtEventdMap::GetInstance()->ChangeMouseEvent(event));
}

/// 鼠标按下
void QtFBOWindow::MousePress(QMouseEvent *event,qreal rScal)
{
    m_rMouseX = event->x()*rScal;
    m_rMouseY = event->y()*rScal;

    auto iter = m_pAllOserver->begin();
    auto tmpIter = iter;

    MouseButtonMask mask = ChangeMouseType(event);
    for(;iter != m_pAllOserver->end(); )
    {
        ++tmpIter;
        (*iter)->MouseDown(mask,m_rMouseX,m_rMouseY);
        iter = tmpIter;
    }

    keyboardModifiers(event);
    getEventQueue()->mouseButtonPress(m_rMouseX, m_rMouseY, QtEventdMap::GetInstance()->ChangeMouseEvent(event));
}

/// 鼠标弹起
void QtFBOWindow::MouseUp(QMouseEvent *event,qreal rScal)
{
    m_rMouseX = event->x()*rScal;
    m_rMouseY = event->y()*rScal;

    auto iter = m_pAllOserver->begin();
    auto tmpIter = iter;

    MouseButtonMask mask = ChangeMouseType(event);
    for(;iter != m_pAllOserver->end(); )
    {
        ++tmpIter;
        (*iter)->MouseUp(mask,m_rMouseX,m_rMouseY);
        iter = tmpIter;
    }

    keyboardModifiers(event);
    getEventQueue()->mouseButtonRelease(m_rMouseX, m_rMouseY, QtEventdMap::GetInstance()->ChangeMouseEvent(event));
}

/// 鼠标移动
void QtFBOWindow::MouseMove(QMouseEvent *event,qreal rScal)
{
    m_rMouseX = event->x()*rScal;
    m_rMouseY = event->y()*rScal;

    auto iter = m_pAllOserver->begin();
    auto tmpIter = iter;
    MouseButtonMask mask = ChangeMouseType(event);
    for(;iter != m_pAllOserver->end(); )
    {
        ++tmpIter;
        (*iter)->MouseMove(mask,m_rMouseX,m_rMouseY);
        iter = tmpIter;
    }

    keyboardModifiers(event);
    getEventQueue()->mouseMotion(m_rMouseX, m_rMouseY);
}

/// qml的鼠标移动
void QtFBOWindow::HoverMove(QHoverEvent *event, qreal rScal)
{
    m_rMouseX = event->pos().x()*rScal;
    m_rMouseY = event->pos().y()*rScal;

    auto iter = m_pAllOserver->begin();
    auto tmpIter = iter;
    for(;iter != m_pAllOserver->end(); )
    {
        ++tmpIter;
        (*iter)->MouseMove(UNKNOWN_BUTTON,m_rMouseX,m_rMouseY);
        iter = tmpIter;
    }

    keyboardModifiers(event);
    getEventQueue()->mouseMotion(m_rMouseX, m_rMouseY);
}

/// 拖拽消息
void QtFBOWindow::Drop(QDropEvent *event, qreal rScal)
{
    m_rMouseX = event->pos().x()*rScal;
    m_rMouseY = event->pos().y()*rScal;

    auto iter = m_pAllOserver->begin();
    auto tmpIter = iter;
    for(;iter != m_pAllOserver->end(); )
    {
        ++tmpIter;
        (*iter)->MouseMove(UNKNOWN_BUTTON,m_rMouseX,m_rMouseY);
        iter = tmpIter;
    }

    getEventQueue()->mouseMotion(m_rMouseX, m_rMouseY);
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
                osgEvent = getEventQueue()->touchBegan(id, osgGA::GUIEventAdapter::TOUCH_BEGAN,
                                                       static_cast<float>(touchPoint.pos().x()),
                                                       static_cast<float>(touchPoint.pos().y()));
            }
            else
            {
                osgEvent->addTouchPoint(id, osgGA::GUIEventAdapter::TOUCH_BEGAN,
                                        static_cast<float>(touchPoint.pos().x()),
                                        static_cast<float>(touchPoint.pos().y()));
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
                                                       static_cast<float>(touchPoint.pos().x()),
                                                       static_cast<float>(touchPoint.pos().y()), tap_count);
            }
            else
            {
                osgEvent->addTouchPoint(id, osgGA::GUIEventAdapter::TOUCH_ENDED,
                                        static_cast<float>(touchPoint.pos().x()),
                                        static_cast<float>(touchPoint.pos().y()));
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
                                                       static_cast<float>(touchPoint.pos().x()),
                                                       static_cast<float>(touchPoint.pos().y()));
            }
            else
            {
                osgEvent->addTouchPoint(id, osgGA::GUIEventAdapter::TOUCH_MOVED,
                                        static_cast<float>(touchPoint.pos().x()),
                                        static_cast<float>(touchPoint.pos().y()));
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
                                                       static_cast<float>(touchPoint.pos().x()),
                                                       static_cast<float>(touchPoint.pos().y()));
            }
            else
            {
                osgEvent->addTouchPoint(id, osgGA::GUIEventAdapter::TOUCH_STATIONERY,
                                        static_cast<float>(touchPoint.pos().x()),
                                        static_cast<float>(touchPoint.pos().y()));
            }
            ++id;
        }
    }
        break;
    }
}
