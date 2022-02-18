#ifndef INTERFACE_WINDOW_H
#define INTERFACE_WINDOW_H

#include <vector>
class QWindow;
class QWidget;
class QQuickItem;
class IViewPort;
struct ScenePos;

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

struct TouchInfo
{
    int nId;
    int nX;
    int nY;
};

/**
 * @brief 窗口消息
 * @attention 所有的消息均在GUI的主线程中运行
 */
class IWindowMessageObserver
{
public:
    virtual ~IWindowMessageObserver(){}

    /**
     * @brief 鼠标消息
     * @param MouseButtonMask 鼠标按下的类型
     * @param 第一个int   当前鼠标按下点的屏幕位置的X坐标
     * @param 第二个int   当前鼠标按下点的屏幕位置的Y坐标
     */
    virtual void MouseDown(MouseButtonMask, int, int){}
    virtual void MouseUp(MouseButtonMask, int, int){}
    virtual void MouseMove(MouseButtonMask, int, int){}
    virtual void MouseDblClick(MouseButtonMask, int, int){}

    /**
     * @brief 滚轮消息
     */
    virtual void WheelMove(int){}

    /**
     * @brief 触屏消息
     * @param 触控点个数,以及每个点的信息
     */
    virtual void TouchPointPress(int,std::vector<TouchInfo>&){}
    virtual void TouchPointReleased(int,std::vector<TouchInfo>&){}
    virtual void TouchPointMove(int,std::vector<TouchInfo>&){}

    /**
     * @brief 按键消息
     * @param 除了ASCII码，剩下的都是Qt的Key
     * @param 第二个为 按下的Qt组合键
     */
    virtual void KeyDown(int,int){}
    virtual void KeyUp(int,int){}
};

/**
 * @brief 窗口的基类
 */
class IWindow
{
public:
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
protected:
    virtual ~IWindow(){}
};



#endif
