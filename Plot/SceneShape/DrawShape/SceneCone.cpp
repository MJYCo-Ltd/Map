#include <Sofa/sofam.h>
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
    }

    m_pVertexArray->at(j).set(dR ,0,1.f);
}

/// 创建图形
void CSceneCone::CreateShape()
{
    m_pVertexArray->resize(122);
    m_pColorArray->resize(122);
    UpdateShape();
    UpdateColor();

    m_pGeometry->setColorArray(m_pColorArray,osg::Array::BIND_PER_VERTEX);
    osg::DrawArrays* pDrawCone = new osg::DrawArrays(GL_TRIANGLE_FAN,0,m_pVertexArray->size());
    osg::DrawArrays* pDrawLine = new osg::DrawArrays(GL_LINE_STRIP,1,m_pVertexArray->size()-1);
    m_pGeometry->addPrimitiveSet(pDrawCone);
    m_pGeometry->addPrimitiveSet(pDrawLine);
}

void CSceneCone::UpdateColor()
{
    m_pColorArray->at(0).set(m_stColor.fR,m_stColor.fG,m_stColor.fB,1.f);
    for(int i=1;i<122;++i)
    {
        m_pColorArray->at(i).set(m_stColor.fR,m_stColor.fG,m_stColor.fB,m_stColor.fA);
    }
}
