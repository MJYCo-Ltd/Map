#include "SceneLine.h"

void CSceneLine::CreateShape()
{
    m_pPreviousPoints = new osg::Vec3Array;
    m_pPreviousPoints->setBinding(osg::Array::BIND_PER_VERTEX);
    m_pGeometry->setVertexAttribArray(osg::Drawable::ATTRIBUTE_6, m_pPreviousPoints);

    m_pNextPoints = new osg::Vec3Array;
    m_pNextPoints->setBinding(osg::Array::BIND_PER_VERTEX);
    m_pGeometry->setVertexAttribArray(osg::Drawable::ATTRIBUTE_7, m_pNextPoints);

    m_pDrawArrays=new osg::DrawArrays(GL_LINE_STRIP,0,m_pVertexArray->size());
    m_pGeometry->addPrimitiveSet(m_pDrawArrays);

}

void CSceneLine::UpdateShape()
{
    if(m_bCountChanged)
    {
        if(m_listAllPos.Size() > 1)
        {
            ImplSceneGeometry<ILine>::UpdateShape();

            /// 更新上一个顶点和下一个顶点的值
            int nSize = m_pVertexArray->size();
            if(nSize > 1)
            {
                m_pPreviousPoints->resize(nSize);
                m_pNextPoints->resize(nSize);

                for(int i=0;i<nSize;++i)
                {
                    if(0 == i)
                    {
                        m_pPreviousPoints->at(0).set(m_pVertexArray->at(0));
                        m_pNextPoints->at(0).set(m_pVertexArray->at(0));
                    }
                    else if((nSize-1)==i)
                    {
                        m_pPreviousPoints->at(i).set(m_pVertexArray->at(i-1));
                        m_pNextPoints->at(i-1).set(m_pVertexArray->at(i));
                        m_pNextPoints->at(i).set(m_pVertexArray->at(i));
                    }
                    else
                    {
                        m_pPreviousPoints->at(i).set(m_pVertexArray->at(i-1));
                        m_pNextPoints->at(i-1).set(m_pVertexArray->at(i));
                    }
                }

                m_pPreviousPoints->dirty();
                m_pNextPoints->dirty();
            }
        }

        m_bCountChanged=false;
    }

    if(m_bWidthChanged)
    {
        m_pProgramNode->setStateSet(m_pSceneGraph->ResouceLoader()->GetOrCreateStateSet("GLSL/Line.glsl"));

        m_pGeometry->getOrCreateStateSet()->getOrCreateUniform("LineWidth",osg::Uniform::FLOAT)->set(static_cast<float>(m_nLineWidth));
        m_bWidthChanged=false;
    }

    if(m_bGlowChanged)
    {
        if(m_bOpenGlow)
        {
            m_pProgramNode->setStateSet(m_pSceneGraph->ResouceLoader()->GetOrCreateStateSet("GLSL/Line.glsl"));
            m_pGeometry->getOrCreateStateSet()->setDefine("LINE_GLOW");
        }
        else
        {
            m_pGeometry->getOrCreateStateSet()->removeDefine("LINE_GLOW");
        }
        m_bGlowChanged=false;
    }

}
