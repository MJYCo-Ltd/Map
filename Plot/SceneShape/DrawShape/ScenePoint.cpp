#include <osg/PointSprite>
#include "ScenePoint.h"

/// 更新形状
void CScenePoint::UpdateShape()
{
    ImplSceneGeometry<IPoint>::UpdateShape();

    /// 点大小更改
    if(m_bPointSizeChanged)
    {
        if(m_ufPointSize.valid())
        {
            m_ufPointSize->set(m_fPointSize);
        }
        m_bPointSizeChanged = false;
    }

    /// 图片路径修改
    if(m_bImageChanged)
    {
        auto pPointTexture = m_pSceneGraph->ResouceLoader()->LoadTexture(m_sImagePath);
        if(nullptr != pPointTexture)
        {
            m_pGeometry->getOrCreateStateSet()->setTextureAttributeAndModes(0, pPointTexture);
        }
        else
        {
            m_pGeometry->getOrCreateStateSet()->removeTextureAttribute(0, osg::StateAttribute::TEXTURE);
        }
        m_bImageChanged= false;
    }
}

/// 创建形状
void CScenePoint::CreateShape()
{
    m_pGeometry->setCullingActive(false);
    auto pSate = m_pGeometry->getOrCreateStateSet();
    auto pNodeProgram = osgEarth::VirtualProgram::getOrCreate(pSate);
    /// 此处应该不知道
    if(m_pSceneGraph->ResouceLoader()->LoadVirtualProgram(pNodeProgram,"GLSL/Point.glsl"))
    {
        /// 获取点大小
        m_ufPointSize = pSate->getOrCreateUniform("pointSize",osg::Uniform::FLOAT);
        m_ufPointSize->set(m_fPointSize);
        pSate->setMode(GL_VERTEX_PROGRAM_POINT_SIZE,1);
        auto pSprite = new osg::PointSprite();
        pSprite->setCoordOriginMode(osg::PointSprite::LOWER_LEFT);
        pSate->setTextureAttributeAndModes(0, pSprite, osg::StateAttribute::ON);
    }


    m_pDrawArrays=new osg::DrawArrays(GL_POINTS,0,m_pVertexArray->size());
    m_pGeometry->addPrimitiveSet(m_pDrawArrays);
}
