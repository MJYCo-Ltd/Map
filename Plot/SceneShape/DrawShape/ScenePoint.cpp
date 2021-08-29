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
    auto pStateSet = m_pSceneGraph->ResouceLoader()->LoadVirtualProgram("GLSL/Point.glsl");
    auto pNodeState = m_pGeometry->getStateSet();
    if(nullptr == pNodeState)
    {
        pNodeState = new osg::StateSet;
    }

    /// 获取点大小
    m_ufPointSize = pNodeState->getOrCreateUniform("pointSize",osg::Uniform::FLOAT);
    m_ufPointSize->set(m_fPointSize);
    pNodeState->setMode(GL_VERTEX_PROGRAM_POINT_SIZE,1);
    auto pSprite = new osg::PointSprite();
    pSprite->setCoordOriginMode(osg::PointSprite::LOWER_LEFT);
    pNodeState->setTextureAttributeAndModes(0, pSprite, osg::StateAttribute::ON);


    m_pDrawArrays=new osg::DrawArrays(GL_POINTS,0,m_pVertexArray->size());
    m_pGeometry->addPrimitiveSet(m_pDrawArrays);
    m_pGeometry->setStateSet(m_pSceneGraph->ResouceLoader()->MergeStateSet(pStateSet,pNodeState));
}
