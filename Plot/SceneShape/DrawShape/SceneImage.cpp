#include "SceneImage.h"

void CSceneImage::ImageSizeChanged()
{
    if(m_stImageSize)
    {
        m_stImageSize.bOutSet = true;
        m_bSizeChanged=true;
        ShapeChanged();
    }
}

void CSceneImage::CreateShape()
{
    m_pProgramNode->setCullingActive(false);
    m_pTexCoordArray = new osg::Vec2Array;
    m_pVertexArray->resize(4);
    m_pTexCoordArray->resize(4);
    osg::DrawArrays* pDrawCone = new osg::DrawArrays(GL_TRIANGLE_STRIP,0,m_pVertexArray->size());
    m_pGeometry->addPrimitiveSet(pDrawCone);
    m_pGeometry->setTexCoordArray(0,m_pTexCoordArray);

    m_pTexCoordArray->at(1).set(0,1);
    m_pTexCoordArray->at(2).set(1,0);
    m_pTexCoordArray->at(3).set(1,1);

    auto pStateSet = m_pSceneGraph->ResouceLoader()->GetOrCreateStateSet("GLSL/Global.glsl");
    m_pProgramNode->setStateSet(pStateSet);
}

void CSceneImage::UpdateShape()
{

    if(m_bPathChanged)
    {
        auto pTexture = m_pSceneGraph->ResouceLoader()->LoadTexture(m_sImagePath);
        if(nullptr != pTexture)
        {
            auto pNodeState = m_pGeometry->getOrCreateStateSet();
            pNodeState->setTextureAttributeAndModes(0,pTexture);

            if(!m_stImageSize || !m_stImageSize.bOutSet)
            {
                m_stImageSize.unWidth = pTexture->getImage()->s();
                m_stImageSize.unHeight = pTexture->getImage()->t();
                m_bSizeChanged = true;
            }
        }

        m_bPathChanged=false;
    }

    if(m_bSizeChanged)
    {
        int nX(m_stImageSize.unWidth/2),nY(m_stImageSize.unHeight/2);

        m_pVertexArray->at(0).set(-nX,-nY,0);
        m_pVertexArray->at(1).set(-nX,nY,0);
        m_pVertexArray->at(2).set(nX,-nY,0);
        m_pVertexArray->at(3).set(nX,nY,0);

        m_bSizeChanged=false;
    }

    if(m_bImageDataChanged)
    {
        int size = m_stRGBAData.unWidth*4;
        unsigned char* pTempBuffer = new unsigned char[size]();
        if(m_stRGBAData.bFlipVertically)
        {
            for(int i=m_stRGBAData.unHeight-1,j=0; i>j; --i,++j)
            {
                memcpy(pTempBuffer,m_stRGBAData.pRGBAData+j*size,size);
                memcpy(m_stRGBAData.pRGBAData+j*size,m_stRGBAData.pRGBAData+i*size,size);
                memcpy(m_stRGBAData.pRGBAData+i*size,pTempBuffer,size);
            }
        }
        delete[]pTempBuffer;

        auto pImage = new osg::Image;
        pImage->setImage(m_stRGBAData.unWidth, m_stRGBAData.unHeight, 1,
                        GL_RGBA,GL_RGBA, GL_UNSIGNED_BYTE,m_stRGBAData.pRGBAData,osg::Image::NO_DELETE);

        auto pTexture = new osg::Texture2D;
        pTexture->setImage(pImage);
        pTexture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
        pTexture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
        pTexture->setResizeNonPowerOfTwoHint(false);

        auto pNodeState = m_pGeometry->getOrCreateStateSet();
        pNodeState->setTextureAttributeAndModes(0,pTexture);

        if(!m_stImageSize || !m_stImageSize.bOutSet)
        {
            m_stImageSize.unWidth = m_stRGBAData.unWidth;
            m_stImageSize.unHeight = m_stRGBAData.unHeight;
            m_bSizeChanged = true;
        }
        m_bImageDataChanged=false;
    }

}
