#include <osgUtil/Tessellator>
#include <osgEarth/Tessellator>
#include "ScenePolygon.h"

/// 创建多边形
void CScenePolygon::CreateShape()
{
    m_pDrawArrays = new osg::DrawArrays(GL_TRIANGLE_FAN,0,m_pVertexArray->size());
    m_pGeometry->addPrimitiveSet(m_pDrawArrays);
}

/// 更新多边形
void CScenePolygon::UpdateShape()
{
    if(m_listAllPos.Size() > 2)
    {
        ImplSceneGeometry<IPolygon>::UpdateShape();
        /// 进行凹多边形裁剪

        m_pGeometry->removePrimitiveSet(0,m_pGeometry->getNumPrimitiveSets());
        m_pGeometry->addPrimitiveSet(m_pDrawArrays);
        osgEarth::Tessellator oeTess;
        if (!oeTess.tessellateGeometry(*m_pGeometry))
        {
            osgUtil::Tessellator tess;
            tess.setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);
            tess.setWindingType(osgUtil::Tessellator::TESS_WINDING_POSITIVE);
            tess.retessellatePolygons(*m_pGeometry);
        }
    }
}
