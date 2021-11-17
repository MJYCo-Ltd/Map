#include "SceneBox.h"

void CSceneBox::UpdateShape()
{
    double dy(m_dYXRatio),dz(m_dZXRatio);

    m_pVertexArray->at(0).set(-1.f,-dy,dz);
    m_pNormals->at(0).set(-1.f,-dy,dz);
    m_pNormals->at(0).normalize();

    m_pVertexArray->at(1).set(-1.f,-dy,-dz);
    m_pNormals->at(1).set(-1.f,-dy,-dz);
    m_pNormals->at(1).normalize();

    m_pVertexArray->at(2).set(1.f,-dy,-dz);
    m_pNormals->at(2).set(1.f,-dy,-dz);
    m_pNormals->at(2).normalize();

    m_pVertexArray->at(3).set(1.f,-dy,dz);
    m_pNormals->at(3).set(1.f,-dy,dz);
    m_pNormals->at(3).normalize();

    m_pVertexArray->at(4).set(1.f,dy,dz);
    m_pNormals->at(4).set(1.f,dy,dz);
    m_pNormals->at(4).normalize();

    m_pVertexArray->at(5).set(1.f,dy,-dz);
    m_pNormals->at(5).set(1.f,dy,-dz);
    m_pNormals->at(5).normalize();

    m_pVertexArray->at(6).set(-1.f,dy,-dz);
    m_pNormals->at(6).set(-1.f,dy,-dz);
    m_pNormals->at(6).normalize();

    m_pVertexArray->at(7).set(-1.f,dy,dz);
    m_pNormals->at(7).set(-1.f,dy,dz);
    m_pNormals->at(7).normalize();
}

/// 创建多边形
void CSceneBox::CreateShape()
{
    m_pVertexArray->resize(8);
    m_pNormals->resize(8);
    UpdateShape();

    osg::ref_ptr<osg::DrawElements> primitives =  new osg::DrawElementsUShort(GL_TRIANGLES);
    m_pGeometry->addPrimitiveSet(primitives.get());


    unsigned int p0,p1,p2,p3;
    /// 前后面
    for(unsigned int i=0; i<8; i+=4)
    {
        p0 = i;
        p1 = i+1;
        p2 = i+2;
        p3 = i+3;
        primitives->addElement(p0);
        primitives->addElement(p1);
        primitives->addElement(p3);

        primitives->addElement(p1);
        primitives->addElement(p2);
        primitives->addElement(p3);
    }

    /// 左右面
    for(unsigned int i=0; i<8; i+=4)
    {
        p0 = i+3;
        p1 = i+2;
        p2 = (i+5)%8;
        p3 = (i+4)%8;

        primitives->addElement(p0);
        primitives->addElement(p1);
        primitives->addElement(p3);

        primitives->addElement(p1);
        primitives->addElement(p2);
        primitives->addElement(p3);
    }

    p0 = 0;
    p1 = 3;
    p2 = 4;
    p3 = 7;

    primitives->addElement(p0);
    primitives->addElement(p1);
    primitives->addElement(p3);

    primitives->addElement(p1);
    primitives->addElement(p2);
    primitives->addElement(p3);

    p0 = 2;
    p1 = 1;
    p2 = 6;
    p3 = 5;

    primitives->addElement(p0);
    primitives->addElement(p1);
    primitives->addElement(p3);

    primitives->addElement(p1);
    primitives->addElement(p2);
    primitives->addElement(p3);
}
