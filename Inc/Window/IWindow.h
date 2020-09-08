#ifndef INTERFACE_WINDOW_H
#define INTERFACE_WINDOW_H

#include "SceneGraph/IViewPort.h"

class QWindow;
class QWidget;
class QQuickItem;

/**
 * @brief 鼠标类型
 */
enum MouseButtonMask
{
    UNKNOWN_BUTTON       = 0,
    LEFT_MOUSE_BUTTON    = 1<<0,
    MIDDLE_MOUSE_BUTTON  = 1<<1,
    RIGHT_MOUSE_BUTTON   = 1<<2
};

/**
 * @brief 窗口消息
 * @attention 所有的消息均在GUI的主线程中运行
 */
struct IWindowMessageObserver
{
    virtual ~IWindowMessageObserver(){}

    /**
     * @brief 鼠标消息
     * @param pWindow       发出消息的窗口
     * @param MouseButtonMask 鼠标按下的类型
     * @param 第一个int   当前鼠标按下点的屏幕位置的X坐标
     * @param 第二个int   当前鼠标按下点的屏幕位置的Y坐标
     */
    virtual void MouseDown(MouseButtonMask, int, int){}
    virtual void MouseUp(MouseButtonMask, int, int){}
    virtual void MouseMove(MouseButtonMask, int, int){}
    virtual void MouseDblClick(MouseButtonMask, int, int){}

    /**
     * @brief 按键消息
     */
    virtual void KeyDown(char){}
    virtual void KeyUp(char){}
};

/**
 * @brief 窗口的基类
 */
struct IWindow
{
    virtual ~IWindow(){}

    /**
     * @brief 设置帧率
     */
    virtual void SetFrameRate(int)=0;
    virtual  int GetFrameRate()=0;

    /**
     * @brief 获取主视点
     * @return
     */
    virtual IViewPort* GetMainViewPoint()=0;

    /**
     * @brief 创建一个新的视点
     * @return
     */
    virtual IViewPort* CreateViewPoint()=0;

    /**
     * @brief 删除一个视点
     * @return
     */
    virtual bool DeleteViewPoint(IViewPort*&)=0;

    /**
     * @brief 变成QWidget窗口
     * @return
     */
    virtual QWidget* ToWidget()=0;

    /**
     * @brief 转换成QWindow
     * @return
     */
    virtual QWindow* ToWindow()=0;

    /**
     * @brief 订阅消息
     */
    virtual bool SubMessage(IWindowMessageObserver*)=0;
    virtual bool UnSubMessage(IWindowMessageObserver*)=0;
};



#endif
