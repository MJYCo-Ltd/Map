#include "SceneRadar.h"

/// 更新形状
void CSceneRadar::UpdateShape()
{
    m_pGeometry->removePrimitiveSet(0,m_pGeometry->getNumPrimitiveSets());

    m_pDrawSideFace->clear();


    double azimuthMin=osg::DegreesToRadians(m_dMinAzim);
    double azimuthMax=osg::DegreesToRadians(m_dMaxAzim);

    double elevationMin=osg::DegreesToRadians(m_dMinElev);
    double elevationMax=osg::DegreesToRadians(m_dMaxElev);

    static double angleSegmentMax=osg::DegreesToRadians(10.0);


    double fRow=(elevationMax-elevationMin)/angleSegmentMax;
    double fColumn=(azimuthMax-azimuthMin)/angleSegmentMax;


    int row=fRow==int(fRow)?fRow:fRow+1;
    ++row;

    int column=fColumn==int(fColumn)?fColumn:fColumn+1;

    /// 最后一个点是原点
    m_pVertexArray->resize(row*column+1);

    int zz=0;

    osg::Vec3 axis(0,1,0);

    for (int i=0;i<row;++i)
    {
        float curElev=std::min(i*angleSegmentMax+elevationMin,elevationMax);

        for (int j=0;j<column;++j)
        {
            float curAzimuth=std::min(j*angleSegmentMax+azimuthMin,azimuthMax);

            osg::Matrixf matrix=osg::Matrix::rotate(curElev,osg::X_AXIS,
                                                    0,osg::Y_AXIS,curAzimuth,osg::Z_AXIS);

            m_pVertexArray->at(zz)=matrix.preMult(axis);
            ++zz;
        }
    }

    for (int i=row-2;i>-1;--i)
    {
        auto pDrawFace = new osg::DrawElementsUShort(GL_TRIANGLE_STRIP);
        for (int j=0;j<column;++j)
        {
            pDrawFace->push_back(i*column+j);
            pDrawFace->push_back((i+1)*column+j);
        }

        m_pGeometry->addPrimitiveSet(pDrawFace);
    }


    for (int i=row-2;i>-1;--i)
    {
        m_pDrawSideFace->push_back(i*column);
        m_pDrawSideFace->push_back((i+1)*column);
        m_pDrawSideFace->push_back(m_pVertexArray->size()-1);

        m_pDrawSideFace->push_back((i+2)*column-1);
        m_pDrawSideFace->push_back((i+1)*column-1);
        m_pDrawSideFace->push_back(m_pVertexArray->size()-1);
    }


    for (int i=column-2;i>-1;--i)
    {
        m_pDrawSideFace->push_back(i);
        m_pDrawSideFace->push_back(i+1);
        m_pDrawSideFace->push_back(m_pVertexArray->size()-1);

        m_pDrawSideFace->push_back((row-1)*column+i);
        m_pDrawSideFace->push_back((row-1)*column+i+1);
        m_pDrawSideFace->push_back(m_pVertexArray->size()-1);
    }

    m_pDrawSideFace->dirty();
    m_pGeometry->addPrimitiveSet(m_pDrawSideFace);
}

/// 创建形状
void CSceneRadar::CreateShape()
{
    m_pDrawSideFace = new osg::DrawElementsUShort(GL_TRIANGLES);

//    m_pGeometry->addPrimitiveSet(m_pDrawSideFace);


    UpdateShape();
}
