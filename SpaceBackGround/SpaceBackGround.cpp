#include <ctime>
#include <osg/Callback>
#include <Math/Matrix.h>
#include <Satellite/Date.h>
#include "Solar/SolarEnv.h"
#include "Star/StarEnv.h"

#include "SpaceBackGround.h"

/// 更新矩阵
class CMatixUpdateCallback:public osg::Callback
{
public:

    CMatixUpdateCallback(CSpaceBackGround* pEnv):m_pSpaceEnv(pEnv){}

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
    CSpaceBackGround* m_pSpaceEnv;
};

///
CSpaceBackGround::CSpaceBackGround(ISceneGraph *pSceneGraph):
    QtOsgSceneNode<ISpaceBackGround>(pSceneGraph)
{
}

CSpaceBackGround::~CSpaceBackGround()
{
}

void CSpaceBackGround::SetMaxVisibleMagnitude(int nMax)
{
}

void CSpaceBackGround::SetMilkywayVisible(bool bShow)
{
}

void CSpaceBackGround::SetStarNamesVisible(bool bShow)
{
    m_pStarEnv->SetStarNameVisible(bShow);
}

void CSpaceBackGround::SetBoundariesVisible(bool bShow)
{
    m_pStarEnv->SetBoundaryVisible(bShow);
}

void CSpaceBackGround::SetConstellationLinesVisible(bool bShow)
{
    m_pStarEnv->SetConstellationVisible(bShow);
}

/// 设置显示
void CSpaceBackGround::SetConstellationNamesVisible(bool bShow)
{
    m_pStarEnv->SetStarNameVisible(bShow);
}

/// 设置行星名称显隐
void CSpaceBackGround::SetPlanetsNamesVisible(bool bShow)
{
    m_pSolarEnv->SetPlanetNameShow(bShow);
}

/// 更新时间
void CSpaceBackGround::UpdateDate(double dMJD)
{
    if(fabs(m_dMJD - dMJD) > 1e-11)
    {
        m_dMJD = dMJD;
        m_pUpdateCallBack->NeedUpdate();
    }
}

/// 初始化场景节点
void CSpaceBackGround::InitSceneNode()
{
    m_pOsgNode = new osg::Group;

    m_pUpdateCallBack = new CMatixUpdateCallback(this);
    m_pOsgNode->addUpdateCallback(m_pUpdateCallBack);

    m_pStarEnv = new CStarEnv(m_pSceneGraph);
    m_pSolarEnv = new CSolarEnv(m_pSceneGraph);
    m_pSolarEnv->CreateSolar();

    m_pOsgNode->addChild(m_pStarEnv);
    m_pOsgNode->addChild(m_pSolarEnv);


    time_t timep;

    /// 更新时间
    time(&timep);
    auto p = gmtime(&timep);
    Aerospace::CDate data(p->tm_year+1900,p->tm_mon+1
                    ,p->tm_mday,p->tm_hour
                    ,p->tm_min,p->tm_sec,UTC);

    UpdateDate(data.GetMJD());
}

void CSpaceBackGround::UpdateMatrix()
{
    m_pSolarEnv->UpdateTime(m_dMJD);
}

/// 创建空间背景
ISpaceBackGround *CreateSpaceEnv(ISceneGraph* pSceneGraph)
{
    return(new CSpaceBackGround(pSceneGraph));
}
