#ifndef QT_OSG_WINDOW_H
#define QT_OSG_WINDOW_H

#include <QOpenGLWindow>

class QtFBOWindow;
class QtRender;
class QOpenGLShaderProgram;
class QOpenGLVertexArrayObject;

class QtOsgWindow : public QOpenGLWindow
{
    Q_OBJECT
public:
    QtOsgWindow(QThread* pThread);
    ~QtOsgWindow();

    /**
     * @brief 关联渲染器
     * @param pRender
     */
    void ConnectRender(QtRender* pRender);

    /**
     * @brief 设置FBO窗口
     * @param pFBOWindow
     */
    void SetFBOWindow(QtFBOWindow* pFBOWindow);
    QtFBOWindow* GetFBOWindow();


public slots:
    void UpdateTexture();

    /// 覆盖父类方法
protected:
    void initializeGL() override;
    void paintUnderGL();
    void timerEvent(QTimerEvent *event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
private:
    void Init();

private:
    QThread*        m_pThread=nullptr;
    QtRender*       m_pRender=nullptr;
    QtFBOWindow*    m_pFBOWindow=nullptr;
    QOpenGLShaderProgram* m_pProgram=nullptr;
    QOpenGLVertexArrayObject *m_pVao=nullptr;
    uint             m_unTextureID;
    int              m_nTimerID;
    bool             m_bInit;
    bool             m_bUpdate;
    bool             m_bOther;
    bool             m_bChild;
};

#endif // GRAPHICSVIEW_H
