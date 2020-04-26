#include <QQuickWindow>
#include "SGSimpleTextureNode.h"

QtOSGSimpleTextureNode::QtOSGSimpleTextureNode(QQuickWindow *pWindow)
    :m_pWindow(pWindow)
    ,m_pTexture(nullptr)
    ,m_nTextureID(0)
    ,m_bNeedUpdate(false)
    ,m_stTextureSize(1,1)
{
    /// 设置纹理属性
    setTextureCoordinatesTransform(QSGSimpleTextureNode::MirrorVertically);

    m_pTexture = m_pWindow->createTextureFromId(m_nTextureID,m_stTextureSize);
    setTexture(m_pTexture);

    setFiltering(QSGTexture::Linear);

}

/// 资源
QtOSGSimpleTextureNode::~QtOSGSimpleTextureNode()
{
    delete  m_pTexture;
}

/// 重设纹理信息
void QtOSGSimpleTextureNode::RebuildTexture()
{
    /// 如果纹理无效则不发送重新绘制消息
    if(m_bNeedUpdate)
    {
        if(nullptr != m_pTexture)
        {
            delete m_pTexture;
        }
        m_pTexture = m_pWindow->createTextureFromId(m_nTextureID,m_stTextureSize);
        m_bNeedUpdate = false;
        setTexture(m_pTexture);
        markDirty(DirtyMaterial);
    }
}

/// 保存需要更新的纹理信息
void QtOSGSimpleTextureNode::UpdateTexture(uint nID)
{
    if(nID != m_nTextureID)
    {
        m_nTextureID  = nID;
        m_bNeedUpdate = (0 != m_nTextureID);
    }
}

/// 更新纹理大小
void QtOSGSimpleTextureNode::UpdateSize(const QSize &rSize)
{
    m_stTextureSize = rSize;
}
