#include <QLibrary>
#include <Math/VecMat.h>
#include <Satellite/CoorSys.h>
#include <Math/YPRAngle.h>
#include <Math/Quaternion.h>
#include <SatelliteToolKit/SatelliteToolKit.h>
#include <Map/SpaceEnv/ISpaceBackGround.h>
#include <ISceneCore.h>
#include <Inner/IRender.h>
#include <Inner/OsgExtern/OsgExtern.h>
#include "SpaceEnv.h"

CSpaceEnv::CSpaceEnv(ISceneGraph *pSceneGraph):
    ImplSceneAttitudeGroup<ISpaceEnv>(pSceneGraph)
{
    m_vAllPos.resize(11);
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
        osg::notify(osg::WARN)<<sErrorInfo<<std::endl;
    }

    m_pSpaceBackGround = dynamic_cast<ISpaceBackGround*>(m_pSceneGraph->GetPlot()->CreateSceneNode("ISpaceBackGround"));

    ImplSceneAttitudeGroup<ISpaceEnv>::InitNode();
    /// 判断是否加载成功
    if(nullptr != m_pSpaceBackGround)
    {
        AddSceneNode(m_pSpaceBackGround);
    }
    else
    {
        m_pMainCamera->setClearMask(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    }
}

#include <Satellite/TimeSys.h>
#include <Satellite/JPLEphemeris.h>
/// 更新时间
void CSpaceEnv::UpdateDate(double dMJD)
{
    m_matAttitude = Aerospace::CCoorSys::ECF2ECI(dMJD);

    m_pOsgAttitudeNode->SetRotateMatrix(m_matAttitude);

    Aerospace::CTimeSys timeSys(dMJD);
    double dMJDTT = timeSys.GetTT();

    m_vAllPos[10] = Aerospace::CJPLEphemeris::GetInstance()->GetPos(dMJDTT,Sun,Earth);
    m_vSun = m_vAllPos[10] * m_matAttitude;

    if(nullptr != m_pSpaceBackGround)
    {
        m_pSpaceBackGround->UpdateMatrix(m_matAttitude);

        for(int nIndex=Mercury;nIndex<Sun;++nIndex)
        {
            if(nIndex != Earth)
            {
                m_vAllPos[nIndex-1] = Aerospace::CJPLEphemeris::GetInstance()
                        ->GetPos(dMJDTT,static_cast<PLANET_TYPE>(nIndex),Earth);
            }
        }

        m_pSpaceBackGround->UpdatePos(m_vAllPos);
    }
}
