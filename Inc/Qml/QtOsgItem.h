#ifndef QT_OSG_ITEM_H
#define QT_OSG_ITEM_H

/**
 * @brief Qt Quick 的场景节点
 */

#include <QQuickItem>
#include "../SceneCore_Global.h"

class QtFBOWindow;
class ISceneGraph;
class QtOSGSimpleTextureNode;

class SCENECORESHARED_EXPORT QtOsgItem:public QQuickItem
{
    Q_OBJECT
public:
    QtOsgItem();
    ~QtOsgItem();

    /**
     * @brief 获取场景图
     * @return
     */
    ISceneGraph* GetSceneGraph();

    /**
     * @brief 获取FBO窗口
     * @return
     */
    QtFBOWindow* GetFBOWindow();

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
   void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);


    /**
     * @brief 获取节点数据
     * @return
     */
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *);

protected slots:
    void UpdateTexture();

private:
    uint                 m_unTextureID;/// 需要更新的纹理
    bool                 m_bUpdate=false;/// 是否更改
    ISceneGraph*         m_pSceneGraph;/// 场景图
    QtFBOWindow*         m_pFBOWindow; /// 渲染窗口
    QtOSGSimpleTextureNode*m_pShow;    /// Item显示的节点
};

#endif // OSGITEM_H
