#include <osgUtil/Optimizer>
#include "SceneImage.h"

struct MyDrawCallBack:public osg::Drawable::DrawCallback
{
    void drawImplementation(osg::RenderInfo& renderInfo,const osg::Drawable* drawable) const
    {
        drawable->drawImplementation(renderInfo);

        auto pLastProgram=renderInfo.getState()->getLastAppliedProgramObject();
        if(nullptr != pLastProgram)
        {

            auto pProgram = pLastProgram->getProgram();
            osg::notify(osg::WARN)<<"====================================================="<<std::endl;
            osg::notify(osg::WARN)<<pProgram->getName()<<"\n--------------------------------------------"<<std::endl;
            for(int i=0;i<pProgram->getNumShaders();++i)
            {
                osg::notify(osg::WARN)<<pProgram->getShader(i)->getName()<<"\n--------------------------------------------"<<std::endl;
                osg::notify(osg::WARN)<<pProgram->getShader(i)->getShaderSource()<<std::endl;
            }

            osg::notify(osg::WARN)<<renderInfo.getState()->getUniformMap().at("baseTexture").uniformVec[0].second<<std::endl;
        }
    }
};

void CSceneImage::ImageSizeChanged()
{
    if(m_stImageSize)
    {
        m_stImageSize.bOutSet = true;
        m_bSizeChanged=true;
        ShapeChanged();
    }
}

void CSceneImage::ImagePathChanged()
{
    m_bPathChanged=true;
    ShapeChanged();
}

void CSceneImage::CreateShape()
{
    m_pGeometry->setDrawCallback(new MyDrawCallBack);
    m_pTexCoordArray = new osg::Vec2Array;
    m_pVertexArray->resize(4);
    m_pTexCoordArray->resize(4);
    UpdateShape();
    osg::DrawArrays* pDrawCone = new osg::DrawArrays(GL_TRIANGLE_STRIP,0,m_pVertexArray->size());
    m_pGeometry->addPrimitiveSet(pDrawCone);
    m_pGeometry->setTexCoordArray(0,m_pTexCoordArray);

    m_pTexCoordArray->at(1).set(0,1);
    m_pTexCoordArray->at(2).set(1,0);
    m_pTexCoordArray->at(3).set(1,1);
}

void CSceneImage::UpdateShape()
{

    if(m_bPathChanged)
    {
        auto pTexture = m_pSceneGraph->ResouceLoader()->LoadTexture(m_sImagePath);
        if(nullptr != pTexture)
        {
            m_pGeometry->getOrCreateStateSet()->setTextureAttributeAndModes(0,pTexture);

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

}
