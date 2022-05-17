#include "SceneDonuts.h"
#include <Inner/OsgExtern/MathCommon.h>
#include <Inner/OsgExtern/Grid2TriangleStrip.h>
#include <VersionMathCommon.h>


void CSceneDonuts::CreateShape()
{
    m_pVertexArray->resizeArray(25*25);
    UpdateShape();
    CGrid2TriangleStrip grid(25,25,true);
    const osg::MixinVector<osg::VectorGLuint>& rIndex = grid.GetIndex();

    for(int nIndex=0; nIndex<rIndex.size(); ++nIndex)
    {
        m_pGeometry->addPrimitiveSet(new osg::DrawElementsUInt(GL_TRIANGLE_STRIP,rIndex[nIndex].size(),&rIndex[nIndex].front()));
    }
    m_bBuild = true;

    //m_pColorArray->push_back(m_vColor);
}

void CSceneDonuts::UpdateShape()
{
    double dR = (m_dRMax - m_dRMin) / 2.0;
    CGrid2TriangleStrip grid(25,25,true);

    const osg::MixinVector<osg::VectorGLuint>& rIndex = grid.GetIndex();
   // m_pGeometry->setVertexArray(m_pVertexArray);

    osg::Vec3 vLocal;
    for(int i=0,indexi=0; i<=360; i +=15,++indexi)
    {
        osg::Matrix tmpMatrix = osg::Matrix::rotate(osg::DegreesToRadians((float)i),osg::Z_AXIS);
        for(int j=0,indexj=0; j<=360; j +=15,++indexj)
        {
            vLocal.z() = CMathCommon::GetInstance()->GetCos()[j] * dR;
            vLocal.x() = m_dRMin+dR + CMathCommon::GetInstance()->GetSin()[j] * dR;
            vLocal.y() = 0;

            vLocal = tmpMatrix.preMult(vLocal);
            m_pVertexArray->at(indexj*25 + indexi) = vLocal;
            //pVetexArray->push_back(vLocal);
        }
    }
}
