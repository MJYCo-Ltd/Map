#ifndef CMYQTWINDOW_H
#define CMYQTWINDOW_H

#include <list>
#include <SceneGraph/IWindow.h>

class QThread;
class QtOsgWindow;
class QtOsgItem;
class QtFBOWindow;
class QtViewPort;
class QtRender;
class ISceneGraph;

class QtWindow:public IWindow
{
public:
    QtWindow(ISceneGraph* pSceneGraph,QtRender* pRender, QThread* pThread,int nType);
    ~QtWindow();

    /**
     * @brief 设置帧率
     */
    void SetFrameRate(int);
    int GetFrameRate(){return(m_nFrameRate);}

    /**
     * @brief 创建一个新的视点
     * @return
     */
    IViewPort* CreateViewPoint();

    /**
     * @brief 获取主视点
     * @return
     */
    IViewPort* GetMainViewPoint();

    /**
     * @brief 删除一个视点
     * @return
     */
     bool DeleteViewPoint(IViewPort*&);

    /**
     * @brief 变成QWidget窗口
     * @return
     */
    virtual QWidget* ToWidget();

    /**
     * @brief 变成QWindow
     * @return
     */
    virtual QWindow* ToWindow();

    /**
     * @brief 获取Osg的窗口
     * @return
     */
    QtFBOWindow* GetOsgWindow(){return(m_pFBOWindow);}

    /**
     * @brief 订阅消息
     * @return
     */
    bool SubMessage(IWindowMessageObserver* pObserver);

    /**
     * @brief 取消订阅
     * @param pObserver
     * @attention 并不会释放订阅者的资源
     * @return
     */
    bool UnSubMessage(IWindowMessageObserver* pObserver);

    /**
     * @brief 初始化窗口
     */
    void InitWindow();
protected:
    ISceneGraph*    m_pSceneGraph;
    QThread*        m_pThread;
    QtRender*       m_pRender;
    QtOsgWindow*    m_pWindow=nullptr;        /// Qt的窗口
    QWidget*        m_pWidget=nullptr;        /// 返回Widget窗口
    QtFBOWindow*    m_pFBOWindow=nullptr;     /// osg窗口
    QtViewPort*    m_pMainViewPoint=nullptr;
    int             m_nType;
    int             m_nFrameRate=60;          /// 帧率
    bool            m_bInit=false;
    bool            m_bCanChange=true;
    std::list<QtViewPort*> m_vOtherViewPoint;
    std::list<IWindowMessageObserver*> m_allWindowMessageObserver;
};

#endif // CMYQTWINDOW_H
