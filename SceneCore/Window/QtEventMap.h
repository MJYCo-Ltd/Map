#ifndef QTEVENTMAP_H
#define QTEVENTMAP_H

#include <map>
#include <osgGA/GUIEventAdapter>

class QKeyEvent;
class QInputEvent;
class QMouseEvent;
class QWheelEvent;
class QTouchEvent;

/**
 * @brief Qt与osg的映射关系
 */
class QtEventdMap
{
public:
    /**
     * @brief 获取单例
     * @return
     */
    static QtEventdMap* GetInstance();

    /**
     * @brief 获取qt 对应的 osg的键值
     * @param event
     * @return
     */
    int ChangeKeyEvent(QKeyEvent* event);

    /**
     * @brief 获取键盘的掩码
     * @param event
     * @return
     */
    int ChangeKeyMask(QInputEvent* event);

    /**
     * @brief 获取鼠标对应的osg的键值
     * @param event
     * @return
     */
    unsigned int ChangeMouseEvent(QMouseEvent* event);

    /**
     * @brief 获取滚轮对应的osg的值
     * @param event
     * @return
     */
    osgGA::GUIEventAdapter::ScrollingMotion ChangeWheelEvent(QWheelEvent* event);

protected:
    QtEventdMap();

protected:
    typedef std::map<int, int> KeyMap;
    KeyMap mKeyMap;
};

#endif // QTEVENTMAP_H
