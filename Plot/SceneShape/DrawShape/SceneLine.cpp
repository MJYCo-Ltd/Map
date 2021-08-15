#include <osgEarth/CullingUtils>
#include "SceneLine.h"

class COsgSceneNodeCullCallback:public osg::Callback
{
public:
    COsgSceneNodeCullCallback(osg::Uniform* pUniform):cameraSize(pUniform){}

    /// 回调函数
    bool run(osg::Object* object, osg::Object* data)
    {
        osg::Node* node = object ? object->asNode() : 0;
        osg::NodeVisitor* nv = data ? data->asNodeVisitor() : 0;
        if(node && nv)
        {
            auto cullVisitor = nv->asCullVisitor();
            if(cullVisitor)
            {
                auto pViewPort = cullVisitor->getViewport();

                if(pViewPort)
                {
                    osg::Vec2 size(pViewPort->width(),pViewPort->height());
                    cameraSize->set(size);
                }
            }
        }
        return traverse(object, data);
    }
private:
    osg::ref_ptr<osg::Uniform> cameraSize;
};

void CSceneLine::CreateShape()
{
    m_nLineWidth=10;
    m_pPreviousPoints = new osg::Vec3Array;
    m_pPreviousPoints->setBinding(osg::Array::BIND_PER_VERTEX);
    m_pGeometry->setVertexAttribArray(osg::Drawable::ATTRIBUTE_6, m_pPreviousPoints);

    m_pNextPoints = new osg::Vec3Array;
    m_pNextPoints->setBinding(osg::Array::BIND_PER_VERTEX);
    m_pGeometry->setVertexAttribArray(osg::Drawable::ATTRIBUTE_7, m_pNextPoints);

    auto pSate = m_pGeometry->getOrCreateStateSet();
    auto pNodeProgram = osgEarth::VirtualProgram::getOrCreate(pSate);
    /// 此处应该不知道
    if(m_pSceneGraph->ResouceLoader()->LoadVirtualProgram(pNodeProgram,"GLSL/Line.glsl"))
    {
        pNodeProgram->addBindAttribLocation("Line_prev_point",osg::Drawable::ATTRIBUTE_6);
        pNodeProgram->addBindAttribLocation("Line_next_point",osg::Drawable::ATTRIBUTE_7);
    }
    m_uLineWidth = pSate->getOrCreateUniform("LineWidth",osg::Uniform::FLOAT);
    m_uLineWidth->set(static_cast<float>(m_nLineWidth));

    m_uCameraSize = pSate->getOrCreateUniform("cameraSize",osg::Uniform::FLOAT_VEC2);

    m_pDrawArrays=new osg::DrawArrays(GL_LINE_STRIP,0,m_pVertexArray->size());
    m_pGeometry->addPrimitiveSet(m_pDrawArrays);
    m_pGeometry->addCullCallback(new COsgSceneNodeCullCallback(m_uCameraSize.get()));

    /// 设置默认状态
    pSate->setMode(GL_CULL_FACE, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE | osg::StateAttribute::PROTECTED);
}

void CSceneLine::UpdateShape()
{
    if(m_bCountChanged)
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

    if(m_bWidthChanged)
    {
        m_uLineWidth->set(static_cast<float>(m_nLineWidth));
        m_bWidthChanged=false;
    }

    if(m_bGlowChanged)
    {
        if(m_bOpenGlow)
        {
            m_pGeometry->getOrCreateStateSet()->setDefine("LINE_GLOW");
        }
        else
        {
            m_pGeometry->getOrCreateStateSet()->removeDefine("LINE_GLOW");
        }
        m_bGlowChanged=false;
    }

}
