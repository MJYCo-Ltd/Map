#include <Math/Intpol.h>
#include <Map/IMap.h>
#include <SpaceEnv/ISpaceEnv.h>
#include <Inner/ILoadResource.h>
#include <Inner/OsgExtern/OsgExtern.h>
#include "Oribit.h"
#include "Satellite3D.h"
#include "Satellite2D.h"
#include "SatelliteShow.h"

string CSatelliteShow::S_sInterFace("ISatellite");



CSatelliteShow::CSatelliteShow(ISceneGraph *pSceneGrap):
    QtOsgEarthMapSceneNode<ISatellite>(pSceneGrap)
{
    m_pOribit = new COribit(m_pSceneGraph);
    //m_p3D = new CSatellite3D(pSceneGrap);
}

void CSatelliteShow::InitSceneNode()
{
    QtOsgEarthMapSceneNode<ISatellite>::InitSceneNode();
    m_pOribit->InitSceneNode();
    /// 添加到节点上去
    m_pSceneGraph->GetMap()->GetSpaceEnv()->AddSceneNode(this);
    m_pSceneGraph->GetMap()->GetSpaceEnv()->AddSceneNode(m_pOribit);
    //m_p3D->BuildName();
}

void CSatelliteShow::SetName(const string & satName)
{
    if(m_sName != satName)
    {
        m_sName = satName;
        //m_p3D->SetSatName(m_sName);
    }
}

void CSatelliteShow::SetModelPath(const string &sModelPath)
{
    if(m_sModelPath != sModelPath)
    {
        m_sModelPath = sModelPath;
        if(m_pSatellite.valid())
        {
            m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CModifyNode(m_pOsgNode,m_pSatellite,false));
        }

        m_pSatellite = m_pSceneGraph->ResouceLoader()->LoadNode(m_sModelPath);
        if(m_pSatellite.valid())
        {
            m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CModifyNode(m_pOsgNode,m_pSatellite,true));
        }
    }
}

void CSatelliteShow::SetOribitColor(const SceneColor &)
{
}

void CSatelliteShow::SetScalBit(double dScal)
{
    if(dScal > 0 && fabs(m_dScal-dScal) > 1e-10)
    {
        m_dScal = dScal;
        m_pUpdataCall->SetScal(m_dScal);
    }
}

/// 设置卫星轨道
void CSatelliteShow::SetJ2000Oribit(const vector<double> &vTime, const vector<Math::CVector> &rOribitInfo)
{
    m_vOribit = rOribitInfo;
    m_vdMjd = vTime;
    m_dStart = vTime[0];
    m_dEnd = vTime[vTime.size()-1];
    m_dStep = vTime[1]-vTime[0];

    m_pOribit->SetJ2000Oribit(rOribitInfo);
}

void CSatelliteShow::SetGeoOribit(const vector<Math::CVector> &vOribitInfo)
{
}

void CSatelliteShow::UpdateData(double dMJD)
{
    static double dTime[3];

    if(fabs(m_dStep)<1e-9 || dMJD > m_dEnd)
    {
        return;
    }

    /// 时间跨度
    double dSpend = dMJD - m_dStart;

    /// 定位数据位置

    int nIndex = dSpend/m_dStep;

    /// 过滤掉非法值
    if(1 > m_nIndex)
    {
        m_nIndex = 1;
    }
    else if(m_nIndex > m_vdMjd.size() -1)
    {
        m_nIndex = m_vdMjd.size() - 1;
    }


    /// 赋值
    dTime[0] = m_vdMjd[m_nIndex-1];
    dTime[1] = m_vdMjd[m_nIndex];
    dTime[2] = m_vdMjd[m_nIndex+1];

    /// 计算插值
    m_stNowPos(0) = CalItNewton(dTime,dMJD,0);
    m_stNowPos(1) = CalItNewton(dTime,dMJD,1);
    m_stNowPos(2) = CalItNewton(dTime,dMJD,2);
    m_stNowPos(3) = CalItNewton(dTime,dMJD,3);
    m_stNowPos(4) = CalItNewton(dTime,dMJD,4);
    m_stNowPos(5) = CalItNewton(dTime,dMJD,5);
    ScenePos tmpPos;
    tmpPos.fX = m_stNowPos(0);
    tmpPos.fY = m_stNowPos(1);
    tmpPos.fZ = m_stNowPos(2);
    tmpPos.bIsGeo = false;

    SetPos(tmpPos);

    Math::CVector pos = m_stNowPos.slice(0,2);
    Math::CVector vPos = m_stNowPos.slice(3,5);

    Math::CVector rZ = -pos;
    Math::CVector rY = vPos;

    rY = Math::CVecMat::Cross(rZ , rY);
    Math::CVector rX = Math::CVecMat::Cross(rY,rZ);

    rX.Normalize();
    rY.Normalize();
    rZ.Normalize();

    /// 设置矩阵
    Math::CMatrix rotate(3,3);
    rotate.SetRow(0,rX);
    rotate.SetRow(1,rY);
    rotate.SetRow(2,rZ);

    Math::CYPRAngle ypr(rotate);
    YPR_Rotate rotateType = ypr.GetRotate(RPY);
    SceneAttitude attitude;
    attitude.dYaw = rotateType.dYaw*DR2D;
    attitude.dRoll = rotateType.dRoll*DR2D;
    attitude.dPitch = rotateType.dPitch*DR2D;
    attitude.rotaOrder = SR_RPY;
    SetAttitude(attitude);
}

/// 计算插值
double CSatelliteShow::CalItNewton(double *dX, double dT, int nDim)
{
    static double dY[3];
    dY[0] = m_vOribit[m_nIndex-1](nDim);
    dY[1] = m_vOribit[m_nIndex](nDim);
    dY[2] = m_vOribit[m_nIndex+1](nDim);

    return(Numerical::Cntpol::ItNewton(3,dX,dY,dT));
}

void CSatelliteShow::BuildName()
{
}

/// 创建节点
ISatellite *CreateNode(ISceneGraph *pSceneGraph, const string &sInterfaceName)
{
    if(sInterfaceName == CSatelliteShow::GetInterFaceName())
    {
        return(new CSatelliteShow(pSceneGraph));
    }
    else
    {
        return(nullptr);
    }
}

bool QueryInterface(string& sInterfaceName)
{
    sInterfaceName = CSatelliteShow::GetInterFaceName();
    return(true);
}
