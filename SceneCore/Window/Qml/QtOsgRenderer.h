#ifndef QTOSGRENDERER_H
#define QTOSGRENDERER_H

#include <QQuickFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <ISceneCore.h>
#include <Qml/QtOsgItem.h>
#include "../QtFBOWindow.h"

class QtOsgRenderer : public QObject,public QQuickFramebufferObject::Renderer
{
    Q_OBJECT
public:
    QtOsgRenderer(ISceneGraph* pSceneGraph);
    ~QtOsgRenderer();

    /**
     * @brief 初始化窗口
     */
    void InitSurface();

    /**
     * @brief 渲染函数
     */
    void render();

    /**
     * @brief 同步信息
     * @param pQuickFBO
     */
    void synchronize(QQuickFramebufferObject *pQuickFBO);
    QtFBOWindow* GetFBOWindow(){return(m_pFBOWindow);}

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size);
public slots:
    void UpdateTexture();
private:
    ISceneGraph*   m_pSceneGraph=nullptr;/// 场景图
    QtOsgItem*           m_pOsgItem=nullptr;
    QtFBOWindow*         m_pFBOWindow=nullptr; /// 渲染窗口
    QOpenGLShaderProgram* m_pProgram=nullptr;
    QOpenGLVertexArrayObject *m_pVao=nullptr;
    QOpenGLFramebufferObject* m_pFBO=nullptr;
    uint                  m_unTextureID=0;
    bool                 m_bInitOpengl=false;
    bool                 m_bInitSys=false;
    bool                 m_bUpdate=false;
};

#endif // QTOSGRENDERER_H
