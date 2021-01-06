#include <osg/PointSprite>
#include "ScenePoint.h"

/// 点大小更改
void CScenePoint::PosChanged()
{
    m_bPosChanged=true;
    ShapeChanged();
}

void CScenePoint::PointSizeChanged()
{
    m_bPointSizeChanged=true;
    ShapeChanged();
}

/// 更新形状
void CScenePoint::UpdateShape()
{
    if(m_bPosChanged)
    {
        m_pVertexArray->at(0).set(m_stPos.fX,m_stPos.fY,m_stPos.fZ);
        m_bPosChanged=false;
    }

    if(m_bPointSizeChanged)
    {
        if(m_ufPointSize.valid())
        {
            m_ufPointSize->set(m_fPointSize);
        }
        m_bPointSizeChanged = false;
    }
}

/// 创建形状
void CScenePoint::CreateShape()
{
    auto pSate = m_pGeometry->getOrCreateStateSet();
    auto pNodeProgram = osgEarth::VirtualProgram::getOrCreate(pSate);
    /// 此处应该不知道
    if(m_pSceneGraph->ResouceLoader()->LoadVirtualProgram(pNodeProgram,"Data/GLSL/Point.glsl"))
    {
        /// 获取闪烁变量
        m_ufPointSize = pSate->getOrCreateUniform("pointSize",osg::Uniform::FLOAT);
        m_ufPointSize->set(1.f);
    }

    m_pVertexArray->push_back(osg::Vec3(m_stPos.fX,m_stPos.fY,m_stPos.fZ));
    m_pGeometry->addPrimitiveSet(new osg::DrawArrays(GL_POINTS,0,m_pVertexArray->size()));

    osg::ref_ptr<osg::Texture2D> starTexture1 = m_pSceneGraph->ResouceLoader()->LoadTexture("SpaceResource/pixmaps/asterism.png");
    pSate->setTextureAttributeAndModes(0, starTexture1.get());
    pSate->setMode(GL_PROGRAM_POINT_SIZE,osg::StateAttribute::ON);
}
