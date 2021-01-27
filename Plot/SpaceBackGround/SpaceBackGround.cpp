#include <ctime>
#include <osg/Callback>
#include <Math/Matrix.h>
#include <Satellite/Date.h>


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
    m_pSkyNode->SetStarNamesVisible(bShow);
}

void CSpaceBackGround::SetBoundariesVisible(bool bShow)
{
    m_pSkyNode->SetBoundariesVisible(bShow);
}

void CSpaceBackGround::SetConstellationLinesVisible(bool bShow)
{
    m_pSkyNode->SetConstellationLinesVisible(bShow);
}

/// 设置显示
void CSpaceBackGround::SetConstellationNamesVisible(bool bShow)
{
    m_pSkyNode->SetConstellationNamesVisible(bShow);
}

/// 设置行星名称显隐
void CSpaceBackGround::SetPlanetsNamesVisible(bool bShow)
{
    m_pSkyNode->SetPlanetsNamesVisible(bShow);
}

/// 更新时间
void CSpaceBackGround::UpdateDate(double dMJD)
{
    m_pSkyNode->UpdateDate(dMJD);
}

/// 初始化场景节点
void CSpaceBackGround::InitNode()
{
    ImplSceneNode<ISpaceBackGround>::InitNode();

    m_pSkyNode = new CSkyNode(m_pSceneGraph);
    SetOsgNode(m_pSkyNode.get());
}

/// 更新矩阵
void CSpaceBackGround::UpdateMatrix(const Math::CMatrix &rRotate)
{
    m_pSkyNode->UpdateMatrix(rRotate);
}

static const char s_sSpaceBackGround[]="ISpaceBackGround";

/// 创建空间背景
ISceneNode *CreateNode(ISceneGraph* pSceneGraph,const std::string& sInterfaceName)
{
    if(sInterfaceName == s_sSpaceBackGround)
    {
        return(new CSpaceBackGround(pSceneGraph));
    }
    else
    {
        return(nullptr);
    }
}

bool QueryInterface(std::string& sInterfaceName)
{
    sInterfaceName = s_sSpaceBackGround;
    return(false);
}
