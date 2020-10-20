#include <Sofa/sofam.h>
#include "SceneSCone.h"

/// 创建模型
void CSceneSCone::CreateShape()
{
    m_pVertexArray->resize(6);
    UpdateShape();
    auto pDrawCone = new osg::DrawArrays(GL_TRIANGLE_FAN,0,m_pVertexArray->size());

    m_pGeometry->addPrimitiveSet(pDrawCone);
}

/// 更新形状
void CSceneSCone::UpdateShape()
{
    double dH = tan(m_dHAngle*DD2R);
    double dV = tan(m_dVAngle*DD2R);

    m_pVertexArray->at(1).set(-dH ,-dV,1.f);
    m_pVertexArray->at(2).set(dH ,-dV,1.f);
    m_pVertexArray->at(3).set(dH ,dV,1.f);
    m_pVertexArray->at(4).set(-dH ,dV,1.f);
    m_pVertexArray->at(5).set(-dH ,-dV,1.f);
}
