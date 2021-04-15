#include "SceneBox.h"

void CSceneBox::UpdateShape()
{
    double dy(m_dYXRatio),dz(m_dZXRatio);
    // -ve y plane
    m_pVertexArray->push_back(osg::Vec3(-1.f,-dy,dz));
    m_pVertexArray->push_back(osg::Vec3(-1.f,-dy,-dz));
    m_pVertexArray->push_back(osg::Vec3(1.f,-dy,-dz));
    m_pVertexArray->push_back(osg::Vec3(1.f,-dy,dz));

    // +ve y plane
    m_pVertexArray->push_back(osg::Vec3(1.f,dy,dz));
    m_pVertexArray->push_back(osg::Vec3(1.f,dy,-dz));
    m_pVertexArray->push_back(osg::Vec3(-1.f,dy,-dz));
    m_pVertexArray->push_back(osg::Vec3(-1.f,dy,dz));

    // +ve x plane
    m_pVertexArray->push_back(osg::Vec3(1.f,-dy,dz));
    m_pVertexArray->push_back(osg::Vec3(1.f,-dy,-dz));
    m_pVertexArray->push_back(osg::Vec3(1.f,dy,-dz));
    m_pVertexArray->push_back(osg::Vec3(1.f,dy,dz));

    // -ve x plane
    m_pVertexArray->push_back(osg::Vec3(-1.f,dy,dz));
    m_pVertexArray->push_back(osg::Vec3(-1.f,dy,-dz));
    m_pVertexArray->push_back(osg::Vec3(-1.f,-dy,-dz));
    m_pVertexArray->push_back(osg::Vec3(-1.f,-dy,dz));

    // +ve z plane
    m_pVertexArray->push_back(osg::Vec3(-1.f,dy,dz));
    m_pVertexArray->push_back(osg::Vec3(-1.f,-dy,dz));
    m_pVertexArray->push_back(osg::Vec3(1.f,-dy,dz));
    m_pVertexArray->push_back(osg::Vec3(1.f,dy,dz));

    // -ve z plane
    m_pVertexArray->push_back(osg::Vec3(1.f,dy,-dz));
    m_pVertexArray->push_back(osg::Vec3(1.f,-dy,-dz));
    m_pVertexArray->push_back(osg::Vec3(-1.f,-dy,-dz));
    m_pVertexArray->push_back(osg::Vec3(-1.f,dy,-dz));
}

/// 创建多边形
void CSceneBox::CreateShape()
{
    m_pVertexArray->reserve(24);
    UpdateShape();

    m_pNormals = new osg::Vec3Array(osg::Array::BIND_PER_VERTEX);
    // -ve y plane
    m_pNormals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
    //TexCoord2f(0.0f,1.0f);

    m_pNormals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
    //TexCoord2f(0.0f,0.0f);

    m_pNormals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
    //TexCoord2f(1.0f,0.0f);

    m_pNormals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
    //TexCoord2f(1.0f,1.0f);

    // +ve y plane
    m_pNormals->push_back(osg::Vec3(0.0f,1.0f,0.0f));
    //TexCoord2f(0.0f,1.0f);

    m_pNormals->push_back(osg::Vec3(0.0f,1.0f,0.0f));
    //TexCoord2f(0.0f,0.0f);

    m_pNormals->push_back(osg::Vec3(0.0f,1.0f,0.0f));
    //TexCoord2f(1.0f,0.0f);

    m_pNormals->push_back(osg::Vec3(0.0f,1.0f,0.0f));
    //TexCoord2f(1.0f,1.0f);

    // +ve x plane
    m_pNormals->push_back(osg::Vec3(1.0f,0.0f,0.0f));
    //TexCoord2f(0.0f,1.0f);

    m_pNormals->push_back(osg::Vec3(1.0f,0.0f,0.0f));
    //TexCoord2f(0.0f,0.0f);

    m_pNormals->push_back(osg::Vec3(1.0f,0.0f,0.0f));
    //TexCoord2f(1.0f,0.0f);

    m_pNormals->push_back(osg::Vec3(1.0f,0.0f,0.0f));
    //TexCoord2f(1.0f,1.0f);

    // -ve x plane
    m_pNormals->push_back(osg::Vec3(-1.0f,0.0f,0.0f));
    //TexCoord2f(0.0f,1.0f);

    m_pNormals->push_back(osg::Vec3(-1.0f,0.0f,0.0f));
    //TexCoord2f(0.0f,0.0f);

    m_pNormals->push_back(osg::Vec3(-1.0f,0.0f,0.0f));
    //TexCoord2f(1.0f,0.0f);

    m_pNormals->push_back(osg::Vec3(-1.0f,0.0f,0.0f));
    //TexCoord2f(1.0f,1.0f);

    // +ve z plane
    m_pNormals->push_back(osg::Vec3(0.0f,0.0f,1.0f));
    //TexCoord2f(0.0f,1.0f);

    m_pNormals->push_back(osg::Vec3(0.0f,0.0f,1.0f));
    //TexCoord2f(0.0f,0.0f);

    m_pNormals->push_back(osg::Vec3(0.0f,0.0f,1.0f));
    //TexCoord2f(1.0f,0.0f);

    m_pNormals->push_back(osg::Vec3(0.0f,0.0f,1.0f));
    //TexCoord2f(1.0f,1.0f);

    // -ve z plane
    m_pNormals->push_back(osg::Vec3(0.0f,0.0f,-1.0f));
    //TexCoord2f(0.0f,1.0f);

    m_pNormals->push_back(osg::Vec3(0.0f,0.0f,-1.0f));
    //TexCoord2f(0.0f,0.0f);

    m_pNormals->push_back(osg::Vec3(0.0f,0.0f,-1.0f));
    //TexCoord2f(1.0f,0.0f);

    m_pNormals->push_back(osg::Vec3(0.0f,0.0f,-1.0f));
    //TexCoord2f(1.0f,1.0f);

    osg::ref_ptr<osg::DrawElements> primitives =  static_cast<osg::DrawElements*>(new osg::DrawElementsUShort(GL_TRIANGLES));
    m_pGeometry->addPrimitiveSet(primitives.get());
    m_pGeometry->setNormalArray(m_pNormals);


    for(unsigned int i=0; i<24; i+=4)
    {
        unsigned int p0 = i;
        unsigned int p1 = i+1;
        unsigned int p2 = i+2;
        unsigned int p3 = i+3;

        primitives->addElement(p0);
        primitives->addElement(p1);
        primitives->addElement(p3);

        primitives->addElement(p1);
        primitives->addElement(p2);
        primitives->addElement(p3);
    }
}
