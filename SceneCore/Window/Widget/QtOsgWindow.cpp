#include <QCoreApplication>
#include <QThread>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QResizeEvent>
#include <osg/Notify>
#include "QtOsgWindow.h"
#include "../QtFBOWindow.h"
#include "../MyShader.h"
#include "SceneGraph/QtRender.h"


QtOsgWindow::QtOsgWindow(QThread *pThread, int nType)
    :m_pOsgRenderThread(pThread)
    ,m_nType(nType)
{
    Init();
}

QtOsgWindow::~QtOsgWindow()
{
    killTimer(m_nTimerID);
    context()->makeCurrent(this);
    delete m_pProgram;
    delete m_pVao;
    context()->doneCurrent();
}

/// 设置渲染类
void QtOsgWindow::ConnectRender(QtRender *pRender)
{
    m_pOsgRender = pRender;
    connect(m_pOsgRender,&QtRender::RenderAFrame,this,&QtOsgWindow::UpdateTexture);
}

void QtOsgWindow::SetFBOWindow(QtFBOWindow *pFBOWindow)
{
    m_pFBOWindow = pFBOWindow;
}

QtFBOWindow *QtOsgWindow::GetFBOWindow()
{
    return(m_pFBOWindow);
}

/// 修改定时器间隔
void QtOsgWindow::ChangeTimer(int nTimer)
{
    if(-1 != m_nTimerID)
    {
        killTimer(m_nTimerID);
        m_nTimerID = startTimer(nTimer);
    }
}

/// 引擎渲染成功返回的纹理
void QtOsgWindow::UpdateTexture()
{
    m_unTextureID = m_pFBOWindow->GetFBOTextureID();
    m_bUpdate = true;
}

void QtOsgWindow::initializeGL()
{
    /// 初始化设备上下文
    context()->doneCurrent();
    m_pFBOWindow->InitContext(context());
    m_pFBOWindow->InitSurface(m_pOsgRenderThread);

    QCoreApplication::postEvent(m_pOsgRender,new RenderResize(m_pFBOWindow,this->size()));

    if(0 == m_nType)
    {
        QCoreApplication::postEvent(m_pOsgRender,new QEvent(static_cast<QEvent::Type>(RENDER_START)));
    }

    context()->makeCurrent(this);

    m_pProgram = new QOpenGLShaderProgram;
    m_pVao = new QOpenGLVertexArrayObject;
    m_pVao->create();

    m_pProgram->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_pProgram->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);

    if(!m_pProgram->link())
    {
        OSG_FATAL<<m_pProgram->log().toUtf8().data();
    }
}

/// 绘制Graphics的背景
void QtOsgWindow::paintUnderGL()
{
    if(0 != m_unTextureID)
    {
        QOpenGLFunctions *f = context()->functions();
        f->glViewport(0,0,this->width() * this->devicePixelRatio(), this->height() * this->devicePixelRatio());
        f->glBindTexture(GL_TEXTURE_2D, m_unTextureID);
        m_pProgram->bind();
        QOpenGLVertexArrayObject::Binder vaoBinder(m_pVao);
        f->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        m_pProgram->release();

        /// 只有当拿到最新的纹理，才会发送绘制下一帧的消息
        if(m_bUpdate)
        {
            QCoreApplication::postEvent(m_pOsgRender,new QEvent(static_cast<QEvent::Type>(RENDER_START)));
            m_bUpdate = false;
        }
    }
}

/// 定时消息
void QtOsgWindow::timerEvent(QTimerEvent *event)
{
    if(m_nTimerID == event->timerId())
    {
        requestUpdate();
    }
}

void QtOsgWindow::keyPressEvent(QKeyEvent *event)
{
    if(nullptr != m_pFBOWindow)
    {
        m_pFBOWindow->KeyPress(event);
    }

    QOpenGLWindow::keyPressEvent(event);
}

void QtOsgWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(nullptr != m_pFBOWindow)
    {
        m_pFBOWindow->KeyUp(event);
    }
    QOpenGLWindow::keyReleaseEvent(event);
}

void QtOsgWindow::resizeEvent(QResizeEvent *event)
{
    QCoreApplication::postEvent(m_pOsgRender, new RenderResize(m_pFBOWindow,event->size()));
    QOpenGLWindow::resizeEvent(event);
}

void QtOsgWindow::mousePressEvent(QMouseEvent *event)
{
    if(nullptr != m_pFBOWindow)
    {
        m_pFBOWindow->MousePress(event,devicePixelRatio());
    }
    QOpenGLWindow::mousePressEvent(event);
}

void QtOsgWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(nullptr != m_pFBOWindow)
    {
        m_pFBOWindow->MouseUp(event,devicePixelRatio());
    }

    QOpenGLWindow::mouseReleaseEvent(event);
}

void QtOsgWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(nullptr != m_pFBOWindow)
    {
        m_pFBOWindow->MouseDouble(event,devicePixelRatio());
    }

    QOpenGLWindow::mouseDoubleClickEvent(event);
}

void QtOsgWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(nullptr != m_pFBOWindow)
    {
        m_pFBOWindow->MouseMove(event,devicePixelRatio());
    }

    QOpenGLWindow::mouseMoveEvent(event);
}

void QtOsgWindow::wheelEvent(QWheelEvent *event)
{
    if(nullptr != m_pFBOWindow)
    {
        m_pFBOWindow->WheelEvent(event);
    }

    QOpenGLWindow::wheelEvent(event);
}

void QtOsgWindow::touchEvent(QTouchEvent *event)
{
    if(nullptr != m_pFBOWindow)
    {
        m_pFBOWindow->TouchEvent(event);
    }

    QOpenGLWindow::touchEvent(event);
}

/// 初始化GraphicsView
void QtOsgWindow::Init()
{
    m_nTimerID = startTimer(16);
}
