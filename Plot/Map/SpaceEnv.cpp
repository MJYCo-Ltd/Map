#include <QLibrary>
#include <Math/VecMat.h>
#include <Satellite/CoorSys.h>
#include <Math/YPRAngle.h>
#include <Math/Quaternion.h>
#include <Satellite/Date.h>
#include <SatelliteToolKit/SatelliteToolKit.h>
#include <Map/SpaceEnv/ISpaceBackGround.h>
#include <ISceneCore.h>
#include <Inner/IRender.h>
#include <Inner/OsgExtern/OsgExtern.h>
#include "SpaceEnv.h"

CSpaceEnv::CSpaceEnv(ISceneGraph *pSceneGraph):
    ImplSceneAttitudeGroup<ISpaceEnv>(pSceneGraph)
{
}

/// 设置主相机
void CSpaceEnv::SetMainCamara(osg::Camera *pCamera)
{
    m_pMainCamera = pCamera;
    m_pMainCamera->setClearMask(GL_DEPTH_BUFFER_BIT);
}

ISpaceBackGround *CSpaceEnv::GetSpaceBackGround()
{
    return(m_pSpaceBackGround);
}

/// 控制空间背景的显隐
void CSpaceEnv::ShowSpaceBackGround(bool bShow)
{
    if(bShow != m_bShowSpaceBackGround)
    {
        m_bShowSpaceBackGround = bShow;
        m_pSpaceBackGround->SetVisible(m_bShowSpaceBackGround);


        if(m_bShowSpaceBackGround)
        {
            m_pMainCamera->setClearMask(GL_DEPTH_BUFFER_BIT);
        }
        else
        {
            m_pMainCamera->setClearMask(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
        }
    }
}

/// 初始化节点
void CSpaceEnv::InitNode()
{
    std::string sErrorInfo;
    if(!InitSatelliteToolKit(GetExePath(),sErrorInfo))
    {
        std::cout<<sErrorInfo<<std::endl;
    }

    m_pSpaceBackGround = dynamic_cast<ISpaceBackGround*>(m_pSceneGraph->GetPlot()->CreateSceneNode("ISpaceBackGround"));

    ImplSceneAttitudeGroup<ISpaceEnv>::InitNode();
    /// 判断是否加载成功
    if(nullptr != m_pSpaceBackGround)
    {
        AddSceneNode(m_pSpaceBackGround);

        time_t timep;

        /// 更新时间
        time(&timep);
        auto p = gmtime(&timep);
        Aerospace::CDate data(p->tm_year+1900,p->tm_mon+1
                              ,p->tm_mday,p->tm_hour
                              ,p->tm_min,p->tm_sec,UTC);

        UpdateDate(data.GetMJD());
    }
    else
    {
        m_pMainCamera->setClearMask(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    }
}

/// 更新时间
void CSpaceEnv::UpdateDate(double dMJD)
{
    m_matAttitude = Aerospace::CCoorSys::ECF2ECI(dMJD);

    AttitudeMatrixChanged();

    if(nullptr != m_pSpaceBackGround)
    {
        m_pSpaceBackGround->UpdateMatrix(m_matAttitude);
        m_pSpaceBackGround->UpdateDate(dMJD);
    }
}

/// 加载星空背景
void CSpaceEnv::LoadBackGround()
{
    if(nullptr == m_pSpaceBackGround)
    {
    }
}
