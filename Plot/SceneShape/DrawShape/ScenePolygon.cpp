#include "ScenePolygon.h"

/// 创建多边形
void CScenePolygon::CreateShape()
{
    m_pTess = new osgUtil::Tessellator;
    m_pTess->setTessellationType(osgUtil::Tessellator::TESS_TYPE_DRAWABLE);
    m_pTess->setWindingType(osgUtil::Tessellator::TESS_WINDING_ODD);

    m_pDrawArrays = new osg::DrawArrays(GL_TRIANGLE_FAN,0,m_pVertexArray->size());
    m_pGeometry->addPrimitiveSet(m_pDrawArrays);
}

/// 更新多边形
void CScenePolygon::UpdateShape()
{
    ImplSceneGeometry<IPolygon>::UpdateShape();
    /// 进行凹多边形裁剪
    m_pTess->retessellatePolygons(*m_pGeometry);
}
