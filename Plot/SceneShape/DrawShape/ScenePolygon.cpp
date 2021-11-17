#include <osgUtil/Tessellator>
#include "ScenePolygon.h"

/// 创建多边形
void CScenePolygon::UpdateVetrix()
{
    ImplSceneGeometry<IPolygon>::UpdateVetrix();

    if(m_pDrawLine.valid())
    {
        m_pDrawLine->setCount(m_pVertexArray->size());
    }
}

void CScenePolygon::CreateShape()
{
    m_pDrawArrays = new osg::DrawArrays(GL_TRIANGLE_FAN,0,m_pVertexArray->size());
    m_pDrawLine = new osg::DrawArrays(GL_LINE_LOOP,0,m_pVertexArray->size());
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
        if(m_bFill)
        {
            m_pGeometry->addPrimitiveSet(m_pDrawArrays);

            /// 防止多边形为凹多边形
            {
                osgUtil::Tessellator tess;
                tess.setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);
                tess.setWindingType(osgUtil::Tessellator::TESS_WINDING_ODD);
                tess.retessellatePolygons(*m_pGeometry);
            }
        }

        if(m_bShowOutLine)
        {
            m_pGeometry->addPrimitiveSet(m_pDrawLine);
        }
    }
}
