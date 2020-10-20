#include <Sofa/sofam.h>
#include "SceneSCone.h"

/// 创建模型
void CSceneSCone::CreateShape()
{
    m_pColorArray->resize(6);
    m_pVertexArray->resize(6);

    UpdateShape();
    UpdateColor();

    m_pGeometry->setColorArray(m_pColorArray,osg::Array::BIND_PER_VERTEX);
    auto pDrawCone = new osg::DrawArrays(GL_TRIANGLE_FAN,0,m_pVertexArray->size());
    auto pDrawLine = new osg::DrawArrays(GL_LINE_STRIP,1,m_pVertexArray->size()-1);

    auto pDrawLineElement = new osg::DrawElementsUShort(GL_LINES);
    pDrawLineElement->reserve(8);
    for(int i=1;i<5;++i)
    {
        pDrawLineElement->push_back(0);
        pDrawLineElement->push_back(i);
    }

    m_pGeometry->addPrimitiveSet(pDrawCone);
    m_pGeometry->addPrimitiveSet(pDrawLine);
    m_pGeometry->addPrimitiveSet(pDrawLineElement);
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

void CSceneSCone::UpdateColor()
{
    m_pColorArray->at(0).set(m_stColor.fR,m_stColor.fG,m_stColor.fB,1.f);
    for(int i=1;i<6;++i)
    {
        m_pColorArray->at(i).set(m_stColor.fR,m_stColor.fG,m_stColor.fB,m_stColor.fA);
    }
}
