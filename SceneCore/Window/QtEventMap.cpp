#include <QKeyEvent>
#include <osgGA/GUIEventAdapter>

#include "QtEventMap.h"

QtEventdMap *QtEventdMap::GetInstance()
{
    static QtEventdMap s_QtKeyboradMap;
    return(&s_QtKeyboradMap);
}

/// 获取映射关系
int QtEventdMap::ChangeKeyEvent(QKeyEvent *event)
{
    KeyMap::iterator itr = mKeyMap.find(event->key());
    if (itr == mKeyMap.end())
    {
        if(!event->text().isEmpty())
        {
            return int(*(event->text().toLatin1().data()));
        }
        else
        {
            return(event->key());
        }
    }
    else
    {
        return (itr->second);
    }
}

/// 获取键盘的掩码
int QtEventdMap::ChangeKeyMask(QInputEvent *event)
{
    static unsigned int mask = 0;

    /// 只取 shit ctrl 和 alt
     int modkey = event->modifiers() & (Qt::SHIFT | Qt::CTRL | Qt::ALT);

    if (modkey & Qt::SHIFT)
    {
        mask |= osgGA::GUIEventAdapter::MODKEY_SHIFT;
    }

    if (modkey & Qt::CTRL)
    {
        mask |= osgGA::GUIEventAdapter::MODKEY_CTRL;
    }

    if (modkey & Qt::ALT)
    {
        mask |= osgGA::GUIEventAdapter::MODKEY_ALT;
    }

    return(static_cast<int>(mask));

}

/// 转换鼠标
unsigned int QtEventdMap::ChangeMouseEvent(QMouseEvent *event)
{
    static unsigned int button = 0;
    switch (event->button())
    {
    case Qt::LeftButton: button = 1; break;
    case Qt::MiddleButton: button = 2; break;
    case Qt::RightButton: button = 3; break;
    default: button = 0; break;
    }

    return(button);
}

/// 转换滚轮的值
osgGA::GUIEventAdapter::ScrollingMotion QtEventdMap::ChangeWheelEvent(QWheelEvent *event)
{
    static osgGA::GUIEventAdapter::ScrollingMotion emMotion;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    if(event->orientation() == Qt::Vertical)
    {
        emMotion = event->delta() > 0 ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN;
    }
    else
    {
        emMotion = event->delta() > 0 ? osgGA::GUIEventAdapter::SCROLL_LEFT : osgGA::GUIEventAdapter::SCROLL_RIGHT;
    }
#else
    QPoint point = event->angleDelta();
    if(point.x() != 0)
    {
        emMotion = point.x() > 0 ? osgGA::GUIEventAdapter::SCROLL_LEFT : osgGA::GUIEventAdapter::SCROLL_RIGHT;
    }

    if(point.y() != 0)
    {
        emMotion = point.y() > 0 ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN;
    }
#endif

    return(emMotion);
}

/// 构建映射关系
QtEventdMap::QtEventdMap()
{
    mKeyMap[Qt::Key_Escape     ] = osgGA::GUIEventAdapter::KEY_Escape;
    mKeyMap[Qt::Key_Delete   ] = osgGA::GUIEventAdapter::KEY_Delete;
    mKeyMap[Qt::Key_Home       ] = osgGA::GUIEventAdapter::KEY_Home;
    mKeyMap[Qt::Key_Enter      ] = osgGA::GUIEventAdapter::KEY_KP_Enter;
    mKeyMap[Qt::Key_End        ] = osgGA::GUIEventAdapter::KEY_End;
    mKeyMap[Qt::Key_Return     ] = osgGA::GUIEventAdapter::KEY_Return;
    mKeyMap[Qt::Key_PageUp     ] = osgGA::GUIEventAdapter::KEY_Page_Up;
    mKeyMap[Qt::Key_PageDown   ] = osgGA::GUIEventAdapter::KEY_Page_Down;
    mKeyMap[Qt::Key_Left       ] = osgGA::GUIEventAdapter::KEY_Left;
    mKeyMap[Qt::Key_Right      ] = osgGA::GUIEventAdapter::KEY_Right;
    mKeyMap[Qt::Key_Up         ] = osgGA::GUIEventAdapter::KEY_Up;
    mKeyMap[Qt::Key_Down       ] = osgGA::GUIEventAdapter::KEY_Down;
    mKeyMap[Qt::Key_Backspace  ] = osgGA::GUIEventAdapter::KEY_BackSpace;
    mKeyMap[Qt::Key_Tab        ] = osgGA::GUIEventAdapter::KEY_Tab;
    mKeyMap[Qt::Key_Space      ] = osgGA::GUIEventAdapter::KEY_Space;
    mKeyMap[Qt::Key_Delete     ] = osgGA::GUIEventAdapter::KEY_Delete;
    mKeyMap[Qt::Key_Alt      ] = osgGA::GUIEventAdapter::KEY_Alt_L;
    mKeyMap[Qt::Key_Shift    ] = osgGA::GUIEventAdapter::KEY_Shift_L;
    mKeyMap[Qt::Key_Control  ] = osgGA::GUIEventAdapter::KEY_Control_L;
    mKeyMap[Qt::Key_Meta     ] = osgGA::GUIEventAdapter::KEY_Meta_L;

    mKeyMap[Qt::Key_F1             ] = osgGA::GUIEventAdapter::KEY_F1;
    mKeyMap[Qt::Key_F2             ] = osgGA::GUIEventAdapter::KEY_F2;
    mKeyMap[Qt::Key_F3             ] = osgGA::GUIEventAdapter::KEY_F3;
    mKeyMap[Qt::Key_F4             ] = osgGA::GUIEventAdapter::KEY_F4;
    mKeyMap[Qt::Key_F5             ] = osgGA::GUIEventAdapter::KEY_F5;
    mKeyMap[Qt::Key_F6             ] = osgGA::GUIEventAdapter::KEY_F6;
    mKeyMap[Qt::Key_F7             ] = osgGA::GUIEventAdapter::KEY_F7;
    mKeyMap[Qt::Key_F8             ] = osgGA::GUIEventAdapter::KEY_F8;
    mKeyMap[Qt::Key_F9             ] = osgGA::GUIEventAdapter::KEY_F9;
    mKeyMap[Qt::Key_F10            ] = osgGA::GUIEventAdapter::KEY_F10;
    mKeyMap[Qt::Key_F11            ] = osgGA::GUIEventAdapter::KEY_F11;
    mKeyMap[Qt::Key_F12            ] = osgGA::GUIEventAdapter::KEY_F12;
    mKeyMap[Qt::Key_F13            ] = osgGA::GUIEventAdapter::KEY_F13;
    mKeyMap[Qt::Key_F14            ] = osgGA::GUIEventAdapter::KEY_F14;
    mKeyMap[Qt::Key_F15            ] = osgGA::GUIEventAdapter::KEY_F15;
    mKeyMap[Qt::Key_F16            ] = osgGA::GUIEventAdapter::KEY_F16;
    mKeyMap[Qt::Key_F17            ] = osgGA::GUIEventAdapter::KEY_F17;
    mKeyMap[Qt::Key_F18            ] = osgGA::GUIEventAdapter::KEY_F18;
    mKeyMap[Qt::Key_F19            ] = osgGA::GUIEventAdapter::KEY_F19;
    mKeyMap[Qt::Key_F20            ] = osgGA::GUIEventAdapter::KEY_F20;

    mKeyMap[Qt::Key_hyphen         ] = '-';
    mKeyMap[Qt::Key_Equal         ] = '=';

    mKeyMap[Qt::Key_division      ] = osgGA::GUIEventAdapter::KEY_KP_Divide;
    mKeyMap[Qt::Key_multiply      ] = osgGA::GUIEventAdapter::KEY_KP_Multiply;
    mKeyMap[Qt::Key_Minus         ] = '-';
    mKeyMap[Qt::Key_Plus          ] = '+';
    mKeyMap[Qt::Key_Insert        ] = osgGA::GUIEventAdapter::KEY_KP_Insert;
}
