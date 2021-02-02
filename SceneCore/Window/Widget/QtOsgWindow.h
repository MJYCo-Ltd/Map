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
    QtOsgWindow(QThread* pThread,int nType);
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

    /**
     * @brief 更改定时器间隔
     */
    void ChangeTimer(int);


public slots:
    void UpdateTexture();

    /// 覆盖父类方法
protected:
    void initializeGL() override;
    void paintUnderGL() override;
    void timerEvent(QTimerEvent *event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
private:
    void Init();

private:
    QThread*        m_pOsgRenderThread=nullptr;
    QtRender*       m_pOsgRender=nullptr;
    QtFBOWindow*    m_pFBOWindow=nullptr;
    QOpenGLShaderProgram* m_pProgram=nullptr;
    QOpenGLVertexArrayObject *m_pVao=nullptr;
    uint             m_unTextureID=0;
    int              m_nType;
    int              m_nTimerID=0;
    bool             m_bInit=false;
    bool             m_bUpdate=true;
};

#endif // GRAPHICSVIEW_H
