#include <osgUtil/Optimizer>
#include "SceneImage.h"

void CSceneImage::ImageSizeChanged()
{
    m_bSizeChanged=true;
    ShapeChanged();
}

void CSceneImage::ImagePathChanged()
{
    m_bPathChanged=true;
    ShapeChanged();
}

void CSceneImage::CreateShape()
{
    m_pTexCoordArray = new osg::Vec2Array;
    m_pVertexArray->resize(4);
    UpdateShape();
    osg::DrawArrays* pDrawCone = new osg::DrawArrays(GL_QUADS,0,m_pVertexArray->size());
    m_pGeometry->addPrimitiveSet(pDrawCone);
    m_pGeometry->setTexCoordArray(0,m_pTexCoordArray);
}

void CSceneImage::UpdateShape()
{
}
