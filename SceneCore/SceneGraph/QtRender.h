#ifndef QTRENDER_H
#define QTRENDER_H

#include <QObject>
#include <QEvent>
#include <QSize>
#include <osgViewer/CompositeViewer>
#include <Inner/IRender.h>

class QtFBOWindow;
enum RenderEvent
{
    RENDER_INIT = QEvent::Type(QEvent::User + 1),
    RENDER_START,
    RENDER_RESIZE,
    RENDER_STOP
};

/**
 * @brief 窗口大小改变的消息
 */
class RenderResize:public QEvent
{
public:
    RenderResize(QtFBOWindow *pFBOWindow, const QSize& rSize);

    QtFBOWindow* GetSender(){return(m_pFBOWindow);}
    const QSize& GetSize(){return(m_stSize);}
protected:
    QtFBOWindow *m_pFBOWindow;
    QSize    m_stSize;
};

class QtRender : public QObject,public IRender
{
    friend class QtSceneGraph;
    Q_OBJECT
public:
    explicit QtRender(QObject *parent = nullptr);
    ~QtRender();

    /**
     * @brief 增加视图
     * @param pView
     */
    void AddView(osgViewer::View* pView);
    void RemoveView(osgViewer::View* pView);

    /**
     * @brief 添加更新操作
     * @param pUpdate
     */
    void AddUpdateOperation(osg::Operation* pUpdate);

signals:
    void RenderAFrame();

private:
    bool event(QEvent *e);

    /**
     * @brief 重置窗口
     * @param event
     */
    void Resize(RenderResize *pRenderResize);
    void Render();
    void Release();
private:
    osg::ref_ptr<osgViewer::CompositeViewer> m_pOsgViewer;    /// 多视口查看器
};

#endif // QTRENDER_H
