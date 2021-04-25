#include <Inner/OsgExtern/JudgeGeometry.h>
#include "ScenePoint.h"

/// 更新形状
void CScenePoint::UpdateShape()
{
    ImplSceneGeometry<IPoint>::UpdateShape();

    if(m_bPointSizeChanged)
    {
        if(m_ufPointSize.valid())
        {
            m_ufPointSize->set(m_fPointSize);
        }
        m_bPointSizeChanged = false;
    }

    if(m_bImageChanged)
    {
        osg::ref_ptr<osg::Texture2D> starTexture1 = m_pSceneGraph->ResouceLoader()->LoadTexture(m_sImagePath);
        if(nullptr != starTexture1)
        {
            m_pGeometry->getOrCreateStateSet()->setTextureAttributeAndModes(0, starTexture1.get());
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
    m_pGeometry->setDrawCallback(new CJudgeGeometry("SCenePoint"));
    auto pSate = m_pGeometry->getOrCreateStateSet();
    auto pNodeProgram = osgEarth::VirtualProgram::getOrCreate(pSate);
    /// 此处应该不知道
    if(m_pSceneGraph->ResouceLoader()->LoadVirtualProgram(pNodeProgram,"GLSL/Point.glsl"))
    {
        /// 获取闪烁变量
        m_ufPointSize = pSate->getOrCreateUniform("pointSize",osg::Uniform::FLOAT);
        m_ufPointSize->set(m_fPointSize);
        pSate->setMode(GL_VERTEX_PROGRAM_POINT_SIZE,1);
    }


    m_pDrawArrays=new osg::DrawArrays(GL_POINTS,0,m_pVertexArray->size());
    m_pGeometry->addPrimitiveSet(m_pDrawArrays);
}
