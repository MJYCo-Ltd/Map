#include <ctime>
#include <osg/Callback>
#include <Math/Matrix.h>
#include <Satellite/Date.h>
#include "Solar/SolarEnv.h"
#include "Star/StarEnv.h"

#include "SpaceBackGround.h"

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
        m_pSolarEnv->UpdateTime(m_dMJD);
    }
}

/// 初始化场景节点
void CSpaceBackGround::InitNode()
{
    m_pOsgNode = new osg::Group;
    SetOsgNode(m_pOsgNode);

    m_pStarEnv = new CStarEnv(ImplSceneNode<ISpaceBackGround>::m_pSceneGraph);
    m_pSolarEnv = new CSolarEnv(ImplSceneNode<ISpaceBackGround>::m_pSceneGraph);
    m_pSolarEnv->CreateSolar();

    m_pOsgNode->addChild(m_pStarEnv);
    m_pOsgNode->addChild(m_pSolarEnv);
}

void CSpaceBackGround::UpdateMatrix(const Math::CMatrix &rRotate)
{
    m_pStarEnv->UpdateMatrix(rRotate);
}

/// 创建空间背景
ISpaceBackGround *CreateSpaceBackGround(ISceneGraph* pSceneGraph)
{
    return(new CSpaceBackGround(pSceneGraph));
}
