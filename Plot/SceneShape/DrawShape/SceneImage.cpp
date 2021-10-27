#include "SceneImage.h"
#include <osgDB/WriteFile>
void CSceneImage::ImageSizeChanged()
{
    if(m_stImageSize)
    {
        m_stImageSize.bOutSet = true;
        SET_TRUE_NODE_UPDATE(m_bSizeChanged)
    }
}

void CSceneImage::SetQImage(const QImage &rImage)
{
    m_pQImageDrawNode = m_pSceneGraph->ResouceLoader()->CreateImageNode(rImage)->asGeometry();
    SET_TRUE_NODE_UPDATE(m_bGeomertyChanged)
}

void CSceneImage::InitNode()
{
    ImplSceneNode<IImage>::InitNode();
    m_bOpenCull=false;
    m_pProgramNode->setCullingActive(m_bOpenCull);
    m_pProgramNode->setStateSet(m_pSceneGraph->ResouceLoader()->GetOrCreateStateSet("GLSL/Global.glsl"));
}

void CSceneImage::FrameCall()
{
    if(m_bGeomertyChanged)
    {
        m_pDrawNode = m_pQImageDrawNode;
        SetOsgNode(m_pDrawNode.get());
        m_bGeomertyChanged = false;
    }

    if(m_bPathChanged)
    {
        m_pDrawNode = m_pSceneGraph->ResouceLoader()->CreateImageNode(m_sImagePath)->asGeometry();
        SetOsgNode(m_pDrawNode.get());
        m_bPathChanged =false;
    }

    if(m_bImageDataChanged)
    {
        m_pDrawNode = m_pSceneGraph->ResouceLoader()->CreateImageNode(&(IImage::m_stRGBAData))->asGeometry();
        SetOsgNode(m_pDrawNode.get());
        m_bImageDataChanged = false;
    }

    if(m_bColorChanged)
    {
        if(m_pDrawNode.valid())
        {
            static_cast<osg::Vec4Array*>(m_pDrawNode->getColorArray())
                    ->at(0).set(m_stColor.fR,m_stColor.fG,m_stColor.fB,m_stColor.fA);
            m_bColorChanged = false;
        }
    }

    if(m_stImageSize.bOutSet)
    {
        if(m_pDrawNode.valid())
        {
            auto pVertexArray = static_cast<osg::Vec3Array*>(m_pDrawNode->getVertexArray());
            int nX(m_stImageSize.unWidth/2);
            int nY(m_stImageSize.unHeight/2);

            pVertexArray->at(0).set(-nX,-nY,0);
            pVertexArray->at(1).set(-nX,nY,0);
            pVertexArray->at(2).set(nX,-nY,0);
            pVertexArray->at(3).set(nX,nY,0);
        }
    }
    ImplSceneNode<IImage>::FrameCall();
}
