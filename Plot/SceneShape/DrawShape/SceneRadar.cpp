#include "SceneRadar.h"

/// 更新形状
void CSceneRadar::UpdateShape()
{
    m_pDrawFace->clear();
    m_pDrawSideFace->clear();
    m_pDrawLine->clear();


    double azimuthMin=osg::DegreesToRadians(m_dMinAzim);
    double azimuthMax=osg::DegreesToRadians(m_dMaxAzim);

    double elevationMin=osg::DegreesToRadians(m_dMinElev);
    double elevationMax=osg::DegreesToRadians(m_dMaxElev);

    static double angleSegmentMax=osg::DegreesToRadians(10.0);


    double fRow=(elevationMax-elevationMin)/angleSegmentMax;
    double fColumn=(azimuthMax-azimuthMin)/angleSegmentMax;


    int row=fRow==(int)fRow?fRow:fRow+1;
    ++row;

    int column=fColumn==(int)fColumn?fColumn:fColumn+1;

    /// 最后一个点是原点
    m_pVertexArray->resize(row*column+1);

    int zz=0;

    osg::Vec3 axis(0,1,0);

    for (int i=0;i!=row;++i)
    {
        float curElev=std::min(i*angleSegmentMax+elevationMin,elevationMax);

        for (int j=0;j!=column;++j)
        {
            float curAzimuth=std::min(j*angleSegmentMax+azimuthMin,azimuthMax);

            osg::Matrixf matrix=osg::Matrix::rotate(curElev,osg::X_AXIS,
                                                    0,osg::Y_AXIS,curAzimuth,osg::Z_AXIS);

            m_pVertexArray->at(zz)=matrix.preMult(axis);
            ++zz;
        }
    }

    for (int i=0;i!=row-1;++i)
    {
        for (int j=0;j!=column-1;++j)
        {
            m_pDrawFace->push_back(i*column+j);
            m_pDrawFace->push_back(i*column+j+1);
            m_pDrawFace->push_back((i+1)*column+j+1);
            m_pDrawFace->push_back((i+1)*column+j);
        }
    }

    for(int i=0;i<row;++i)
    {
        for (int j=0;j<column-1;++j)
        {
            m_pDrawLine->push_back(i*column+j);
            m_pDrawLine->push_back(i*column+j+1);
        }

        if(0==i || row-1 == i)
        {
            for(int j=0;j<column;++j)
            {
                m_pDrawLine->push_back(i*column+j);
                m_pDrawLine->push_back(m_pVertexArray->size()-1);
            }
        }
    }

    for(int i=0;i<row-1;++i)
    {
        for(int j=0;j<column;++j)
        {
            m_pDrawLine->push_back(i*column+j);
            m_pDrawLine->push_back((i+1)*column+j);
        }
    }

    if ((azimuthMax-azimuthMin)<osg::DegreesToRadians(360.0))
    {
        for (int i=0;i!=row-1;++i)
        {
            m_pDrawSideFace->push_back(i*column);
            m_pDrawSideFace->push_back((i+1)*column);
            m_pDrawSideFace->push_back(m_pVertexArray->size()-1);

            m_pDrawSideFace->push_back((i+2)*column-1);
            m_pDrawSideFace->push_back((i+1)*column-1);
            m_pDrawSideFace->push_back(m_pVertexArray->size()-1);
        }
    }

    if (elevationMax-elevationMin<osg::DegreesToRadians(180.0))
    {
        for (int i=0;i!=column-1;++i)
        {
            m_pDrawSideFace->push_back(i);
            m_pDrawSideFace->push_back(i+1);
            m_pDrawSideFace->push_back(m_pVertexArray->size()-1);

            m_pDrawSideFace->push_back((row-1)*column+i);
            m_pDrawSideFace->push_back((row-1)*column+i+1);
            m_pDrawSideFace->push_back(m_pVertexArray->size()-1);
        }
    }

    m_pDrawFace->dirty();
    m_pDrawSideFace->dirty();
    m_pDrawLine->dirty();
}

/// 创建形状
void CSceneRadar::CreateShape()
{
    m_pDrawFace = new osg::DrawElementsUShort(GL_QUADS);
    m_pDrawSideFace = new osg::DrawElementsUShort(GL_TRIANGLES);
    m_pDrawLine = new osg::DrawElementsUShort(GL_LINES);

    m_pGeometry->addPrimitiveSet(m_pDrawFace);
    m_pGeometry->addPrimitiveSet(m_pDrawSideFace);


    UpdateShape();
}
