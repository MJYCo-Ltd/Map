#include <osg/Depth>
#include <Inner/OsgExtern/MathCommon.h>
#include "SceneCone.h"

class ConeCallback:public osg::NodeCallback
{
public:
    ConeCallback(CSceneCone* pCone):
        m_pCone(pCone)
    {
    }

    /// 重写父类
    bool run(osg::Object* object, osg::Object* data)
    {
        if(m_pCone->m_bUpdate)
        {
            double dR = m_pCone->m_dDistance * tan(m_pCone->m_dAngle*DD2R);
            static const double * pCos = CMathCommon::GetInstance()->GetCos();
            static const double* pSin = CMathCommon::GetInstance()->GetSin();

            int j=1;
            for(int i=0; i<360; i += 3,++j)
            {
                m_pVec3Array->at(j).set(dR * pCos[i] ,dR*pSin[i],m_pCone->m_dDistance);
                m_pColorArray->at(j).set(m_pCone->m_stColor.fR,m_pCone->m_stColor.fG,
                                         m_pCone->m_stColor.fB,m_pCone->m_stColor.fA);
            }

            m_pVec3Array->at(j).set(dR ,0,m_pCone->m_dDistance);
            m_pColorArray->at(j) = m_pColorArray->at(0);

            m_pDrawArray->dirty();
            m_pCone->m_bUpdate = false;
        }

        return traverse(object, data);
    }

    void SetVec3Array(osg::Vec3Array* pArray){m_pVec3Array = pArray;}
    void SetVec4Array(osg::Vec4Array* pArray){m_pColorArray = pArray;}
    void SetDrawArrays(osg::DrawArrays* pDrawArrays){m_pDrawArray = pDrawArrays;}


private:
    osg::ref_ptr<osg::Vec3Array> m_pVec3Array;
    osg::ref_ptr<osg::Vec4Array> m_pColorArray;
    osg::ref_ptr<osg::DrawArrays> m_pDrawArray;

    CSceneCone*        m_pCone;
};

///
void CSceneCone::InitSceneNode()
{
    QtSensor<IConeSensor>::InitSceneNode();
    BuildGeometry();
}

CSceneCone::CSceneCone(ISceneGraph *pSceneGraph):
    QtSensor<IConeSensor>(pSceneGraph)
{
}

void CSceneCone::UpdateMapNode(osgEarth::MapNode *pMapNode)
{

}

void CSceneCone::SetAngle(double dAngle)
{
    if(dAngle > 0. && fabs(m_dAngle-dAngle)>1e-10)
    {
        m_dAngle = dAngle;
        m_bUpdate=true;
    }
}

void CSceneCone::BuildGeometry()
{
    auto pLocalGeometry = new osg::Geometry;
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

    double dR = m_dDistance * tan(m_dAngle * DD2R);
    const double * pCos = CMathCommon::GetInstance()->GetCos();
    const double* pSin = CMathCommon::GetInstance()->GetSin();

    osg::Vec4 vColor(m_stColor.fR,m_stColor.fG,
                     m_stColor.fB,m_stColor.fA);
    for(int i=0; i<360; i += 3)
    {
        pArray->push_back(osg::Vec3(dR * pCos[i] ,dR*pSin[i],m_dDistance));
        pColorArray->push_back(vColor);
    }

    pArray->push_back(osg::Vec3(dR ,0,m_dDistance));
    pColorArray->push_back(vColor);

    pLocalGeometry->setVertexArray(pArray);
    pLocalGeometry->setColorArray(pColorArray,osg::Array::BIND_PER_VERTEX);
    osg::DrawArrays* pDrawArray = new osg::DrawArrays(GL_TRIANGLE_FAN,0,pArray->size()) ;
    pLocalGeometry->addPrimitiveSet(pDrawArray);
//    SetVisible(false);
    auto pBack= new ConeCallback(this);
    pBack->SetDrawArrays(pDrawArray);
    pBack->SetVec3Array(pArray);
    pBack->SetVec4Array(pColorArray);

    pLocalGeometry->setUpdateCallback(pBack);
    m_pOsgNode->addChild(pLocalGeometry);
}

void CSceneCone::PosChanged()
{
    if(m_stScenePos.bIsGeo)
    {
    }
}
