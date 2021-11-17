#include <osgUtil/Tessellator>
#include "ScenePolygon3D.h"

void CScenePolygon3D::CreateShape()
{
    MergeStateSet(BLEND_STATE);
}

/// 更新图形
void CScenePolygon3D::UpdateShape()
{
    if(m_bCountChanged)
    {
        if(m_listAllPos.Size() > 2)
        {
            /// 更新数组
            UpdateVetrix();
            UpdateTopFaceVertex();

            int nVertexCount = m_pVertexArray->size()/2;

            /// 移除所有的节点
            m_pGeometry->removePrimitiveSet(0,m_pGeometry->getNumPrimitiveSets());
            if(m_bShowBottom)
            {
                osg::DrawElementsUShort* elBottom = new osg::DrawElementsUShort(GL_TRIANGLE_FAN);
                m_pGeometry->addPrimitiveSet(elBottom);

                for(int nIndex=nVertexCount-1;nIndex>-1;--nIndex)
                {
                    elBottom->push_back(nIndex);
                }
            }

            osg::DrawElementsUShort* elTop = new osg::DrawElementsUShort(GL_TRIANGLE_FAN);
            m_pGeometry->addPrimitiveSet(elTop);
            for(int nIndex=0;nIndex<nVertexCount;++nIndex)
            {
                elTop->push_back(nIndex+nVertexCount);
            }

            /// 根据包围盒中心重新计算法线
            const osg::BoundingBox& rBoundBox = m_pGeometry->getBoundingBox();
            const osg::Vec3f& vCenter = rBoundBox.center();
            m_pNormals->resize(m_pVertexArray->size());
            for(int nIndex=m_pVertexArray->size()-1; nIndex>-1;--nIndex)
            {
                m_pNormals->at(nIndex) = m_pVertexArray->at(nIndex) - vCenter;
                m_pNormals->at(nIndex).normalize();
            }

            /// 防止多边形为凹多边形
            {
                osgUtil::Tessellator tess;
                tess.setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);
                tess.setWindingType(osgUtil::Tessellator::TESS_WINDING_ODD);
                tess.retessellatePolygons(*m_pGeometry);
            }

            /// 绘制边
            osg::DrawElementsUShort* elSide = new osg::DrawElementsUShort(GL_TRIANGLE_STRIP);
            m_pGeometry->addPrimitiveSet(elSide);
            for(int nIndex=0;nIndex<nVertexCount;++nIndex)
            {
                elSide->push_back(nIndex);
                elSide->push_back(nIndex+nVertexCount);
            }

            elSide->push_back(0);
            elSide->push_back(nVertexCount);

        }

        m_bCountChanged = false;
    }

    /// 如果高度改变了
    if(m_bHeightChanged)
    {
        UpdateTopFaceVertex();
        m_bHeightChanged=false;
    }
}

/// 更新顶面的顶点
void CScenePolygon3D::UpdateTopFaceVertex()
{
    int nIndex{0};

    std::vector<ScenePos> vSourcePos;

    if(nullptr != IPolygon3D::m_pDealPoint)
    {
        /// 首先通知形状更改，查看是否需要进行修改
        IPolygon3D::m_pDealPoint->ShapeChanged();

        m_listAllPos.Get(vSourcePos);
        nIndex = vSourcePos.size();
        ImplSceneShape<IPolygon3D>::m_pVertexArray->resize(vSourcePos.size()*2);

        for(ScenePos& rPos : vSourcePos)
        {
            rPos.dZ += m_dHeight;
        }

        std::vector<ScenePos> vAllConverdPos(vSourcePos.size());
        if(IPolygon3D::m_pDealPoint->Conversion(vSourcePos,vAllConverdPos))
        {
            for(auto one : vAllConverdPos)
            {
                ImplSceneShape<IPolygon3D>::m_pVertexArray->at(nIndex++).set(one.dX,one.dY,one.dZ);
            }
        }
    }
    else
    {
        m_listAllPos.Get(vSourcePos);
        nIndex = vSourcePos.size();

        for(ScenePos& rPos : vSourcePos)
        {
            rPos.dZ += m_dHeight;
        }

        ImplSceneShape<IPolygon3D>::m_pVertexArray->resize(vSourcePos.size()*2);
        for(auto one : vSourcePos)
        {
            ImplSceneShape<IPolygon3D>::m_pVertexArray->at(nIndex++).set(one.dX,one.dY,one.dZ);
        }
    }
}
