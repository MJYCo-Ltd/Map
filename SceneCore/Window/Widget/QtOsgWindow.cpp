#include <QCoreApplication>
#include <QThread>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QResizeEvent>
#include <osg/Notify>
#include "QtOsgWindow.h"
#include "../QtFBOWindow.h"
#include "SceneGraph/QtRender.h"


QtOsgWindow::QtOsgWindow(QThread *pThread)
    :m_pThread(pThread)
    ,m_unTextureID(0)
    ,m_nTimerID(0)
    ,m_bInit(false)
    ,m_bUpdate(false)
{
    Init();
}

QtOsgWindow::~QtOsgWindow()
{
    killTimer(m_nTimerID);
}

/// 设置渲染类
void QtOsgWindow::ConnectRender(QtRender *pRender)
{
    m_pRender = pRender;
    connect(m_pRender,&QtRender::RenderAFrame,this,&QtOsgWindow::UpdateTexture);
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
    m_pFBOWindow->InitSurface(m_pThread);

    QCoreApplication::postEvent(m_pRender,new RenderResize(this,this->size()));
    QCoreApplication::postEvent(m_pRender,new QEvent(static_cast<QEvent::Type>(RENDER_START)));

    context()->makeCurrent(this);

    m_pProgram = new QOpenGLShaderProgram;
    m_pVao = new QOpenGLVertexArrayObject;
    m_pVao->create();

    static const char *vertexShaderSource=
            "#version 330\n"
            "out vec2 uv;\n"
            "void main(void)\n"
            "{\n"
                "vec2 pos[] = vec2[](vec2(-1.0, -1.0),\n"
                                    "vec2( 1.0, -1.0),\n"
                                    "vec2(-1.0,  1.0),\n"
                                    "vec2( 1.0,  1.0));\n"
                "vec2 uvpos[] = vec2[](vec2(0, 0.0),\n"
                                    "vec2( 1.0, 0.0),\n"
                                    "vec2(0.0,  1.0),\n"
                                    "vec2( 1.0,  1.0));\n"
                "gl_Position = vec4(pos[gl_VertexID], 0.0, 1.0);\n"
                "uv = uvpos[gl_VertexID];\n"
            "}\n";

    static const char *fragmentShaderSource =
        "in vec2 uv;\n"
        "uniform sampler2D tex;\n"
        "void main() {\n"
        "   gl_FragColor = vec4(texture2D(tex, uv).rgb, 1.0);\n"
        "}\n";

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

        /// 需要更新的时候
        if(m_bUpdate)
        {
            QCoreApplication::postEvent(m_pRender,new QEvent(static_cast<QEvent::Type>(RENDER_START)));
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
    QCoreApplication::postEvent(m_pRender, new RenderResize(this,event->size()));
    QOpenGLWindow::resizeEvent(event);
}

void QtOsgWindow::mousePressEvent(QMouseEvent *event)
{
    if(nullptr != m_pFBOWindow)
    {
        m_pFBOWindow->MousePress(event);
    }
    QOpenGLWindow::mousePressEvent(event);
}

void QtOsgWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(nullptr != m_pFBOWindow)
    {
        m_pFBOWindow->MouseUp(event);
    }

    QOpenGLWindow::mouseReleaseEvent(event);
}

void QtOsgWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(nullptr != m_pFBOWindow)
    {
        m_pFBOWindow->MouseDouble(event);
    }

    QOpenGLWindow::mouseDoubleClickEvent(event);
}

void QtOsgWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(nullptr != m_pFBOWindow)
    {
        m_pFBOWindow->MouseMove(event);
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

#include <osg/GraphicsContext>
#include <osg/DisplaySettings>
/// 初始化GraphicsView
void QtOsgWindow::Init()
{
    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    osg::GraphicsContext::Traits traits(osg::DisplaySettings::instance().get());
    format.setAlphaBufferSize( traits.alpha );
    format.setRedBufferSize( traits.red );
    format.setGreenBufferSize( traits.green );
    format.setBlueBufferSize( traits.blue );
    format.setDepthBufferSize( traits.depth );
    format.setStencilBufferSize( traits.stencil );
    format.setSamples( traits.samples );
//    format.setMajorVersion(3);
//    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);

    format.setSwapInterval( traits.vsync ? 1 : 0 );
    format.setStereo( traits.quadBufferStereo ? 1 : 0 );

    setFormat(format);

    m_nTimerID = startTimer(16);
}
