#ifndef QT_OSG_ITEM_H
#define QT_OSG_ITEM_H

/**
 * @brief Qt Quick 的场景节点
 */

#include <QQuickFramebufferObject>
#include <SceneCore_Global.h>
#include <SceneGraph/ISceneGraph.h>

class QtOsgRenderer;

class SCENECORESHARED_EXPORT QtOsgItem:public QQuickFramebufferObject
{
    friend QtOsgRenderer;
    Q_OBJECT
    Q_PROPERTY(ItemType type MEMBER m_emType WRITE setType)
    Q_PROPERTY(int frameRate MEMBER m_nFrameRate WRITE setFrameRate)

public:
    enum ItemType
    {
        Item_2DMAP,
        Item_3DMAP,
        Item_User
    };
    Q_ENUM(ItemType)

    QtOsgItem(QQuickItem *parent = nullptr);
    ~QtOsgItem();

    /**
     * @brief 设置qml显示的类型
     * @param type
     */
    void setType(ItemType type);

    /**
     * @brief 重写父类窗口
     * @return
     */
    Renderer *createRenderer() const;

    /**
     * @brief 设置帧率
     */
    void setFrameRate(int nFrameRate);

protected slots:
    void Ready();
    void EvnetCall();

protected:
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void touchEvent(QTouchEvent* event);
    void hoverMoveEvent(QHoverEvent* event);
    void dropEvent(QDropEvent *event);

private:
    ItemType       m_emType=Item_3DMAP;  /// 创建类型
    int            m_nFrameRate=60;      /// 帧率
    qreal          m_rScal=1.;           /// 缩放系数
    ISceneGraph*   m_pSceneGraph=nullptr;/// 场景图
    QtOsgRenderer* m_pRenderer=nullptr;  /// 渲染器
};

#endif // OSGITEM_H
