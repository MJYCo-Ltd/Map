#ifndef QT_FBOWINDOW_H
#define QT_FBOWINDOW_H

/**
  *  每一个FBOWindow 与一个窗口对应
   **/

#include<QSize>
#include <QImage>
#include<osgViewer/GraphicsWindow>
#include <list>

class QOpenGLFramebufferObject;
class QOffscreenSurface;
class QOpenGLContext;
class QThread;

class  QMouseEvent;
class QKeyEvent;
class QWheelEvent;
class  QTouchEvent;
class QInputEvent;
class IWindowMessageObserver;

using namespace std;

class QtFBOWindow:public osgViewer::GraphicsWindowEmbedded
{
    friend class QtRender;
public:
    QtFBOWindow(list<IWindowMessageObserver*>* pAllObserver);

    ~QtFBOWindow();

    ////////////////////////////// 实现 osgViewer::GraphicsWindowEmbedded 方法////////////////////////////
    bool makeCurrentImplementation();
    bool releaseContextImplementation();
    void swapBuffersImplementation();
    ////////////////////////////// 实现 osgViewer::GraphicsWindowEmbedded 方法////////////////////////////

    const QSize& GetWinSize(){return(m_stWinSize);}

    /**
     * @brief 初始化设备上下文
     * @param pToShareContext  窗口的设备上下文
     * @attention 是将本窗口的设备上下文
     *                    中的纹理、着色器等 共享
     *                    给 pToShareContext
     */
    bool InitContext(QOpenGLContext *pToShareContext);

    /**
     * @brief 初始化界面
     */
    void InitSurface(QThread *pThread);

    /**
     * @brief 是否初始化
     * @return
     */
    bool IsInit(){return(m_bInit);}

    /**
     * @brief 获取FBO Texture ID
     * @return
     */
    uint GetFBOTextureID(){return(m_nTextureID);}
    const QSize& GetFBOSize(){return(m_stTextureSize);}

    ////////////// 鼠标/键盘/触摸屏 消息处理 ////////////////////////////////////////////
    void keyboardModifiers(QInputEvent* event);
    void KeyPress(QKeyEvent* event);
    void KeyUp(QKeyEvent* event);
    void MouseDouble(QMouseEvent* event);
    void MousePress(QMouseEvent *event);
    void MouseUp(QMouseEvent* event);
    void MouseMove(QMouseEvent* event);
    void WheelEvent(QWheelEvent* event);
    void TouchEvent(QTouchEvent* event);
    ////////////// 鼠标/键盘/触摸屏 消息处理 ////////////////////////////////////////////

protected:

    /**
     * @brief 重置窗口大小
     * @param rWinSize
     */
    void ReSize(const QSize& rWinSize);

    /**
     * @brief 初始化帧缓存对象
     */
    void InitFBO();

private:
    bool                                           m_bInit;          /// 是否已经初始化
    uint                                           m_nInitFBO;       /// 是否初始化 FBO
    uint                                           m_nTextureID;    /// 纹理ID
    QSize                                          m_stWinSize;      /// 窗口大小
    QSize                                          m_stTextureSize;  /// 纹理大小

    QOffscreenSurface                 *m_pOffScreenSurface; /// 离屏渲染表面
    QOpenGLContext                    *m_pOpenglContext;    /// opengl设备上下文
    QOpenGLFramebufferObject *m_pRenderFbo;            /// 渲染帧缓存对象
    QOpenGLFramebufferObject *m_pDisplayFbo;           /// 显示帧缓存对象
    list<IWindowMessageObserver*>*     m_pAllOserver;   /// 所有的消息订阅者
};

#endif // FBOWINDOW_H
