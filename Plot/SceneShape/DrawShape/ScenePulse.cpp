#include "ScenePulse.h"

void CScenePulse::CreateShape()
{
    osg::DrawArrays* pDrawArray = new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP,0,m_pVertexArray->size());
    m_pGeometry->addPrimitiveSet(pDrawArray);
}

void CScenePulse::UpdateShape()
{
}
