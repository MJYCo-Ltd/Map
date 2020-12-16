#ifndef QT_OSG_ITEM_H
#define QT_OSG_ITEM_H

/**
 * @brief Qt Quick 的场景节点
 */

#include <QQuickItem>
#include <SceneCore_Global.h>

class QtFBOWindow;
class ISceneGraph;
class QtOSGSimpleTextureNode;

class SCENECORESHARED_EXPORT QtOsgItem:public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(ItemType type MEMBER m_emType WRITE setType)

public:
    enum ItemType
    {
        Item_2DMAp,
        Item_3DMAP,
        Item_User
    };
    Q_ENUM(ItemType)

    QtOsgItem();
    ~QtOsgItem();

    /**
     * @brief 设置qml显示的类型
     * @param type
     */
    void setType(ItemType type);

    QtFBOWindow* GetFBOWindow(){return(m_pFBOWindow);}

public slots:
    void Ready();

protected:
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void touchEvent(QTouchEvent* event);

protected:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);
#else
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry);
#endif


    /**
     * @brief 获取节点数据
     * @return
     */
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *);

protected slots:
    void UpdateTexture();

private:
    uint                 m_unTextureID=0u;     /// 需要更新的纹理
    bool                 m_bUpdate=false;      /// 是否更改
    ISceneGraph*         m_pSceneGraph=nullptr;/// 场景图
    QtFBOWindow*         m_pFBOWindow=nullptr; /// 渲染窗口
    QtOSGSimpleTextureNode*m_pShow=nullptr;    /// Item显示的节点
    ItemType             m_emType=Item_3DMAP;  /// 创建类型
};

#endif // OSGITEM_H
