#include <osg/Callback>
#include <Math/Matrix.h>
#include <Satellite/CoorSys.h>
#include "Solar/SolarEnv.h"
#include "Star/StarEnv.h"

#include "SpaceEnv.h"

class CMatixUpdateCallback:public osg::Callback
{
public:

    CMatixUpdateCallback(CSpaceEnv* pEnv):m_pSpaceEnv(pEnv){}

    void NeedUpdate()
    {
        m_bNeedUpdate = true;
    }

    bool run(osg::Object* object, osg::Object* data)
    {
        if(m_bNeedUpdate)
        {
            m_pSpaceEnv->UpdateMatrix();
            m_bNeedUpdate = false;
        }
        return osg::Callback::run(object, data);
    }

private:
    bool m_bNeedUpdate=false;
    CSpaceEnv* m_pSpaceEnv;
};


CSpaceEnv::CSpaceEnv(ISceneGraph *pSceneGraph):
    QtSceneNode<ISpaceEnv>(pSceneGraph)
{
}

CSpaceEnv::~CSpaceEnv()
{
}

void CSpaceEnv::SetMaxVisibleMagnitude(int nMax)
{
}

void CSpaceEnv::SetMilkywayVisible(bool bShow)
{
}

void CSpaceEnv::SetStarNamesVisible(bool bShow)
{
    m_pStarEnv->SetStarNameVisible(bShow);
}

void CSpaceEnv::SetBoundariesVisible(bool bShow)
{
    m_pStarEnv->SetBoundaryVisible(bShow);
}

void CSpaceEnv::SetConstellationLinesVisible(bool bShow)
{
    m_pStarEnv->SetConstellationVisible(bShow);
}

void CSpaceEnv::SetConstellationNamesVisible(bool bShow)
{
    m_pStarEnv->SetStarNameVisible(bShow);
}


void CSpaceEnv::SetPlanetsNamesVisible(bool bShow)
{
    m_pSolarEnv->SetPlanetNameShow(bShow);
}

/// 更新时间
void CSpaceEnv::UpdateDate(double dMJD)
{
    if(fabs(m_dMJD - dMJD) > 1e-11)
    {
        m_dMJD = dMJD;
        m_pUpdateCallBack->NeedUpdate();
    }
}

/// 初始化场景节点
void CSpaceEnv::InitSceneNode()
{
    QtSceneNode<ISpaceEnv>::InitSceneNode();
    m_pRotateTransform = new osg::MatrixTransform;
    m_pOsgNode->addChild(m_pRotateTransform);

    m_pUpdateCallBack = new CMatixUpdateCallback(this);
    m_pOsgNode->addUpdateCallback(m_pUpdateCallBack);

    m_pStarEnv = new CStarEnv(m_pSceneGraph);
    m_pSolarEnv = new CSolarEnv(m_pSceneGraph);

	m_pRotateTransform->addChild(m_pStarEnv);
	m_pRotateTransform->addChild(m_pSolarEnv);
}

void CSpaceEnv::UpdateMatrix()
{
    Math::CMatrix ecf2J2000 = Aerospace::CCoorSys::J20002ECF(m_dMJD);
    osg::Matrix rotateMatrix(ecf2J2000(0,0),ecf2J2000(1,0),ecf2J2000(2,0),0.0
                  ,ecf2J2000(0,1),ecf2J2000(1,1),ecf2J2000(2,1),0.0
                  ,ecf2J2000(0,2),ecf2J2000(1,2),ecf2J2000(2,2),0.0
                  ,0.0,0.0,0.0,1.0);

    m_pRotateTransform->setMatrix(rotateMatrix);
}

/// 创建空间背景
ISpaceEnv* CreateSpaceEnv(ISceneGraph* pSceneGraph)
{
    return(new CSpaceEnv(pSceneGraph));
}
