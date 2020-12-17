#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include "QtOsgRenderer.h"
#include "SceneGraph/QtSceneGraph.h"
#include "SceneGraph/QtRender.h"
#include "../QtWindow.h"
#include "../MyShader.h"

QtOsgRenderer::QtOsgRenderer(ISceneGraph *pSceneGraph):
    m_pSceneGraph(pSceneGraph)
{
}

QtOsgRenderer::~QtOsgRenderer()
{
    GetSceneCore()->GetSceneGraphManager()->DeleteSceneGraph(m_pSceneGraph);
}

/// 初始化窗口
void QtOsgRenderer::InitSurface()
{
    m_pFBOWindow->InitSurface(static_cast<QtSceneGraph*>(m_pSceneGraph)->GetRenderThread());
    m_pSceneGraph->Run();
    QtRender* pRender = static_cast<QtRender*>(static_cast<QtSceneGraph*>(m_pSceneGraph)->SceneGraphRender());
    QCoreApplication::postEvent(pRender,new QEvent(static_cast<QEvent::Type>(RENDER_START)));
}

/// 开始渲染
void QtOsgRenderer::render()
{
    if(0 != m_unTextureID)
    {
        QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
        f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        f->glBindTexture(GL_TEXTURE_2D, m_unTextureID);
        m_pProgram->bind();
        QOpenGLVertexArrayObject::Binder vaoBinder(m_pVao);
        f->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        m_pProgram->release();

        /// 需要更新的时候
        if(m_bUpdate)
        {
            QtRender* pRender = static_cast<QtRender*>(static_cast<QtSceneGraph*>(m_pSceneGraph)->SceneGraphRender());
            QCoreApplication::postEvent(pRender,new QEvent(static_cast<QEvent::Type>(RENDER_START)));
            m_bUpdate = false;
        }
    }
}

void QtOsgRenderer::synchronize(QQuickFramebufferObject *pQuickFBO)
{
    m_pOsgItem = qobject_cast<QtOsgItem*>(pQuickFBO);
    if(!m_bInitSys)
    {
        m_pFBOWindow = static_cast<QtWindow*>(m_pSceneGraph->GetMainWindow())->GetOsgWindow();

        QtRender* pRender = static_cast<QtRender*>(static_cast<QtSceneGraph*>(m_pSceneGraph)->SceneGraphRender());
        connect(pRender,&QtRender::RenderAFrame,this,&QtOsgRenderer::UpdateTexture);
        m_pOsgItem->m_pRenderer = this;

        m_bInitSys=true;
    }
}

QOpenGLFramebufferObject *QtOsgRenderer::createFramebufferObject(const QSize &size)
{
    /// 修改窗口大小
    QtRender* pRender = static_cast<QtRender*>(static_cast<QtSceneGraph*>(m_pSceneGraph)->SceneGraphRender());
    QCoreApplication::postEvent(pRender,new RenderResize(m_pFBOWindow,size));

    if(!m_bInitOpengl)
    {
        m_pProgram = new QOpenGLShaderProgram;
        m_pVao = new QOpenGLVertexArrayObject;
        m_pVao->create();

        m_pProgram->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
        m_pProgram->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);

        if(!m_pProgram->link())
        {
            OSG_FATAL<<m_pProgram->log().toUtf8().data();
        }

        m_bInitOpengl=true;

        auto pOpenGLContext = QOpenGLContext::currentContext();
        auto pSurface = pOpenGLContext->surface();
        pOpenGLContext->doneCurrent();
        m_pFBOWindow->InitContext(pOpenGLContext);
        pOpenGLContext->makeCurrent(pSurface);
        QMetaObject::invokeMethod(m_pOsgItem,"Ready");
    }
    qDebug()<<size;
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(4);
    m_pFBO = new QOpenGLFramebufferObject(size, format);
    return m_pFBO;
}

void QtOsgRenderer::UpdateTexture()
{
    m_unTextureID = m_pFBOWindow->GetFBOTextureID();
    m_bUpdate = true;
    QMetaObject::invokeMethod(m_pOsgItem,"update");
}
