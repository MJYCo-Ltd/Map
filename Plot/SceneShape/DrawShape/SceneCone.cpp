#include <VersionMathCommon.h>
#include <Inner/OsgExtern/MathCommon.h>
#include "SceneCone.h"

/// 更新图形
void CSceneCone::UpdateShape()
{
    double dR = tan(m_dAngle*DD2R);
    static const double * pCos = CMathCommon::GetInstance()->GetCos();
    static const double* pSin = CMathCommon::GetInstance()->GetSin();

    int j=1;
    for(int i=0; i<360; i += 3,++j)
    {
        m_pVertexArray->at(j).set(dR * pCos[i] ,dR*pSin[i],1.f);
        m_pNormals->at(j).set(pCos[i],pSin[i],0.f);
    }

    m_pVertexArray->at(j).set(dR ,0,1.f);
}

/// 创建图形
void CSceneCone::CreateShape()
{
    m_pVertexArray->resize(122);
    m_pNormals->resize(122);
    UpdateShape();
    osg::DrawArrays* pDrawCone = new osg::DrawArrays(GL_TRIANGLE_FAN,0,m_pVertexArray->size());
    m_pGeometry->addPrimitiveSet(pDrawCone);
}
