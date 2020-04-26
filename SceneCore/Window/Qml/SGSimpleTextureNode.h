#ifndef SGSIMPLETEXTURENODE_H
#define SGSIMPLETEXTURENODE_H

/**
 * @brief 此类是Qt Quick Scence Graph 场景树上的绘制节点
 *              由渲染线程控制显示的更新
 */

#include<QSGSimpleTextureNode>
#include <QObject>
#include <QMutex>

class QQuickWindow;


class QtOSGSimpleTextureNode : public QObject, public QSGSimpleTextureNode
{
    Q_OBJECT
public:
    QtOSGSimpleTextureNode(QQuickWindow *pWindow);
    ~QtOSGSimpleTextureNode();

    /**
     * @brief 更新纹理
     * @param nID  新的纹理ID
     */
    void UpdateTexture(uint nID);

    /**
     * @brief 更新纹理大小
     * @param rSize
     */
    void UpdateSize(const QSize& rSize);

public slots:

    /**
     * @brief 重新生成纹理
     * @attention 由Scence Graph 渲染流程调用
     */
    void RebuildTexture();


private:
    QQuickWindow* m_pWindow; /// 显示的窗口
    QSGTexture *      m_pTexture;     /// 纹理对象指针
    uint        m_nTextureID;        /// 纹理ID
    bool       m_bNeedUpdate;   /// 是否有效
    QSize m_stTextureSize;     /// 纹理大小
};

#endif // SGSIMPLETEXTURENODE_H
