#include <Math/VecMat.h>
#include <Math/YPRAngle.h>
#include <Math/Quaternion.h>
#include <SatelliteToolKit/SatelliteToolKit.h>
#include <Plot/Map/SpaceEnv/ISpaceBackGround.h>
#include <ISceneCore.h>
#include <Inner/IRender.h>
#include <Inner/OsgExtern/OsgExtern.h>
#include "SpaceEnv.h"

CSpaceEnv::CSpaceEnv(ISceneGraph *pSceneGraph):
    ImplSceneGroup<ISpaceEnv>(pSceneGraph)
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

/// 初始化节点
void CSpaceEnv::InitNode()
{
    std::string sErrorInfo;
    if(!InitSatelliteToolKit(GetExePath(),sErrorInfo))
    {
        osg::notify(osg::WARN)<<sErrorInfo<<std::endl;
    }

    m_pSpaceBackGround = dynamic_cast<ISpaceBackGround*>(m_pSceneGraph->GetPlot()->CreateSceneNode("ISpaceBackGround"));

    ImplSceneGroup<ISpaceEnv>::InitNode();
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
void CSpaceEnv::UpdateDate(double dMJD, const Math::CMatrix& rBackGroundMatrix, const Math::CMatrix &rCaleRote)
{
    Aerospace::CTimeSys timeSys(dMJD);
    double dMJDTT = timeSys.GetTT();

    m_vAllPos[10] = Aerospace::CJPLEphemeris::GetInstance()->GetPos(dMJDTT,Sun,Earth);
    if(rBackGroundMatrix)
    {
        m_vSun = m_vAllPos[10] * rBackGroundMatrix;
    }
    else
    {
        m_vSun = rCaleRote * m_vAllPos[10];
    }

    if(nullptr != m_pSpaceBackGround)
    {
        m_pSpaceBackGround->UpdateMatrix(rBackGroundMatrix);

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

void CSpaceEnv::VisibleChanged()
{
    if(m_bVisible)
    {
        m_pMainCamera->setClearMask(GL_DEPTH_BUFFER_BIT);
    }
    else
    {
        m_pMainCamera->setClearMask(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    }

    ImplSceneGroup<ISpaceEnv>::VisibleChanged();
}
