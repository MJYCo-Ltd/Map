#include <osg/PointSprite>
#include "ScenePoint.h"

/// 更新形状
void CScenePoint::UpdateShape()
{
    ImplSceneGeometry<IPoint>::UpdateShape();

    /// 点大小更改
    if(m_bPointSizeChanged)
    {
        m_pGeometry->getOrCreateStateSet()->getOrCreateUniform("pointSize",osg::Uniform::FLOAT)->set(m_fPointSize);
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
    auto pStateSet = m_pSceneGraph->ResouceLoader()->GetOrCreateStateSet("GLSL/Point.glsl");
    m_pProgramNode->setStateSet(pStateSet);

    m_pDrawArrays=new osg::DrawArrays(GL_POINTS,0,m_pVertexArray->size());
    m_pGeometry->addPrimitiveSet(m_pDrawArrays);
}
