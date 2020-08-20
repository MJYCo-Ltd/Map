#include <osg/Depth>
#include "SceneSCone.h"

class SConeCallback:public osg::Callback
{
public:
    SConeCallback(CSceneSCone* pCone,osg::Geometry* pParent):
        m_pCone(pCone),m_pParent(pParent)
    {
    }

    /// 重写父类
    bool run(osg::Object* object, osg::Object* data)
    {
        if(m_pCone->m_bUpdate)
        {
            double dH = m_pCone->m_dDistance * tan(m_pCone->m_dHAngle*DD2R);
            double dV = m_pCone->m_dDistance * tan(m_pCone->m_dVAngle*DD2R);



            osg::Vec4 color(m_pCone->m_stColor.fR,m_pCone->m_stColor.fG,m_pCone->m_stColor.fB,m_pCone->m_stColor.fA);

            m_pVec3Array->at(1) = osg::Vec3(-dH ,-dV,m_pCone->m_dDistance);
            m_pColorArray->at(1) = color;
            m_pVec3Array->at(2) = osg::Vec3(dH ,-dV,m_pCone->m_dDistance);
            m_pColorArray->at(2) = color;
            m_pVec3Array->at(3) = osg::Vec3(dH ,dV,m_pCone->m_dDistance);
            m_pColorArray->at(3) = color;
            m_pVec3Array->at(4) = osg::Vec3(-dH ,dV,m_pCone->m_dDistance);
            m_pColorArray->at(4) = color;
            m_pVec3Array->at(5) = osg::Vec3(-dH ,-dV,m_pCone->m_dDistance);
            m_pColorArray->at(5) = color;

            m_pParent->dirtyGLObjects();
            m_pCone->m_bUpdate = false;
        }

        return traverse(object, data);
    }

    void SetVec3Array(osg::Vec3Array* pArray){m_pVec3Array = pArray;}
    void SetVec4Array(osg::Vec4Array* pArray){m_pColorArray = pArray;}


private:
    osg::Vec3Array* m_pVec3Array=nullptr;
    osg::Vec4Array* m_pColorArray=nullptr;
    osg::Geometry*  m_pParent=nullptr;
    CSceneSCone*    m_pCone=nullptr;
};

CSceneSCone::CSceneSCone(ISceneGraph *pSceneGraph):
    QtSensor<ISConeSensor>(pSceneGraph)
{
}

void CSceneSCone::InitSceneNode()
{
    QtSensor<ISConeSensor>::InitSceneNode();
    BuildGeometry();
}

void CSceneSCone::SetHAngle(double dHAngle)
{

    if(dHAngle > 0.0 && !osg::equivalent(m_dHAngle,dHAngle))
    {
        m_dHAngle = dHAngle;
        m_bUpdate = true;
    }
}

void CSceneSCone::SetVAngle(double dVAngle)
{
    if(dVAngle > 0.0 && !osg::equivalent(m_dVAngle,dVAngle))
    {
        m_dVAngle = dVAngle;
        m_bUpdate = true;
    }
}

void CSceneSCone::BuildGeometry()
{
    auto pLocalGeometry = new osg::Geometry;
    pLocalGeometry->setDataVariance(osg::Object::DYNAMIC);
    /// 开启颜色混合 关闭光照
    pLocalGeometry->getOrCreateStateSet()->setMode(GL_BLEND,osg::StateAttribute::ON);
    pLocalGeometry->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

    /// 写深度缓存
    osg::Depth* pDepth = new osg::Depth;
    pDepth->setWriteMask(false);
    pLocalGeometry->getOrCreateStateSet()->setAttribute(pDepth);

    osg::ref_ptr<osg::Vec3Array> pArray = new osg::Vec3Array;

    osg::ref_ptr<osg::Vec4Array> pColorArray = new osg::Vec4Array;
    pColorArray->push_back(osg::Vec4(0.5,0.5,0.5,0.5));

    pArray->push_back(osg::Vec3(0.0,0.0,0.0));

    double dH = m_dDistance * tan(m_dHAngle*DD2R);
    double dV = m_dDistance * tan(m_dVAngle*DD2R);
    osg::Vec4 color(m_stColor.fR,m_stColor.fG,m_stColor.fB,m_stColor.fA);


    pArray->push_back(osg::Vec3(-dH ,-dV,m_dDistance));
    pColorArray->push_back(color);
    pArray->push_back(osg::Vec3(dH ,-dV,m_dDistance));
    pColorArray->push_back(color);
    pArray->push_back(osg::Vec3(dH ,dV,m_dDistance));
    pColorArray->push_back(color);
    pArray->push_back(osg::Vec3(-dH ,dV,m_dDistance));
    pColorArray->push_back(color);

    pArray->push_back(osg::Vec3(-dH ,-dV,m_dDistance));
    pColorArray->push_back(color);

    pLocalGeometry->setVertexArray(pArray);
    pLocalGeometry->setColorArray(pColorArray,osg::Array::BIND_PER_VERTEX);
    osg::DrawArrays* pDrawArray = new osg::DrawArrays(GL_TRIANGLE_FAN,0,pArray->size()) ;
    pLocalGeometry->addPrimitiveSet(pDrawArray);
    //    SetVisible(false);
    auto m_pBack= new SConeCallback(this,pLocalGeometry);
    m_pBack->SetVec3Array(pArray);
    m_pBack->SetVec4Array(pColorArray);
    pLocalGeometry->addUpdateCallback(m_pBack);

    m_pOsgNode->addChild(pLocalGeometry);
}

void CSceneSCone::PosChanged()
{
    if(m_stScenePos.bIsGeo)
    {
        osgEarth::GeoPoint geoPos(osgEarth::SpatialReference::get("wgs84"),
                                  m_stScenePos.fLon,m_stScenePos.fLat,m_stScenePos.fHeight,
                                  osgEarth::AltitudeMode::ALTMODE_RELATIVE);
        osg::Matrixd local2World;
        geoPos.createLocalToWorld(local2World);


        m_pUpdataCall->SetPos(local2World.getTrans());
        //
//        osg::Matrixd rotate;
//        local2World.getRotate().get(rotate);
        local2World(3,0) = local2World(3,1) = local2World(3,2)=0;
        m_pUpdataCall->SetMatrix(local2World);
    }
    else
    {
        QtSensor<ISConeSensor>::PosChanged();
    }
}
