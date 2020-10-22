#include <Math/Intpol.h>
#include <Math/VecMat.h>
#include <Plot/SceneShape/ISensor.h>
#include <Plot/SceneShape/ILine.h>
#include <Plot/Common/ISceneAttitudeGroup.h>
#include <Plot/Common/ISceneScaleGroup.h>
#include <Plot/IPlot.h>
#include <Plot/SceneShape/ILine.h>
#include <Plot/Common/ISceneModel.h>
#include <Map/IMap.h>
#include <SpaceEnv/ISpaceEnv.h>
#include "SatelliteShow.h"

string S_3DSatllite("ISatellite");

void CSatelliteShow::InitNode()
{
    m_stNowPos.Resize(6);
    ImplSceneGroup<ISatellite>::InitNode();

    /// 加载模型
    m_pSatellite = m_pSceneGraph->GetPlot()->CreateSceneGroup(ATTITUDE_GROUP)->AsSceneAttitudeGroup();
    m_pOribit = dynamic_cast<ILine*>(m_pSceneGraph->GetPlot()->CreateSceneNode("ILine"));
    AddSceneNode(m_pSatellite);
    AddSceneNode(m_pOribit);

    ///添加到空间节点上去
    m_pSceneGraph->GetMap()->GetSpaceEnv()->AddSceneNode(this);
}


/// 设置卫星轨道
void CSatelliteShow::SetJ2000Oribit(const vector<double> &vTime, const vector<Math::CVector> &rOribitInfo)
{
    m_vOribit = rOribitInfo;
    m_vdMjd = vTime;
    m_dStart = vTime[0];
    m_dEnd = vTime[vTime.size()-1];
    m_dStep = vTime[1]-vTime[0];

    vector<ScenePos> vTemp;
    vTemp.resize(rOribitInfo.size());
    int nIndex(0);
    for(auto iter=rOribitInfo.begin();iter!=rOribitInfo.end();++iter,++nIndex)
    {
        vTemp[nIndex].fX = iter->GetX();
        vTemp[nIndex].fY = iter->GetY();
        vTemp[nIndex].fZ = iter->GetZ();
    }
    m_pOribit->SetMultPos(vTemp);
}

/// 增加传感器
void CSatelliteShow::AddSensor(ISensor *pSensor)
{
    if(m_vSensor.find(pSensor) != m_vSensor.end())
    {
        return;
    }
    m_pSatellite->AddSceneNode(pSensor);
    m_vSensor.insert(pSensor);
}

/// 模型修改
void CSatelliteShow::ModelChanged()
{
    if(nullptr != m_pModel)
    {
        m_pSatellite->RemoveSceneNode(m_pModel);
    }

    m_pModel = m_pSceneGraph->GetPlot()->LoadSceneNode(m_sModelPath)->AsSceneModel();
    auto pGroup = m_pSceneGraph->GetPlot()->CreateSceneGroup(SCALE_GROUP)->AsSceneScaleGroup();
    pGroup->AddSceneNode(m_pModel);
    pGroup->SetScal(10000);
    m_pSatellite->AddSceneNode(pGroup);
}

///名称修改
void CSatelliteShow::NameChanged()
{
}

///更新位置
void CSatelliteShow::NowTimeChanged()
{
    static double dTime[3];

    if(fabs(m_dStep)<1e-9 || m_dNowMJD > m_dEnd)
    {
        return;
    }

    /// 时间跨度
    double dSpend = m_dNowMJD - m_dStart;

    /// 定位数据位置

    int nIndex = dSpend/m_dStep;

    /// 过滤掉非法值
    if(1 > nIndex)
    {
        nIndex = 1;
    }
    else if(nIndex > m_vdMjd.size() -2)
    {
        nIndex = m_vdMjd.size() - 2;
    }

    m_nIndex = nIndex;


    /// 赋值
    dTime[0] = m_vdMjd[m_nIndex-1];
    dTime[1] = m_vdMjd[m_nIndex];
    dTime[2] = m_vdMjd[m_nIndex+1];

    /// 计算插值
    m_stNowPos(0) = CalItNewton(dTime,m_dNowMJD,0);
    m_stNowPos(1) = CalItNewton(dTime,m_dNowMJD,1);
    m_stNowPos(2) = CalItNewton(dTime,m_dNowMJD,2);
    m_stNowPos(3) = CalItNewton(dTime,m_dNowMJD,3);
    m_stNowPos(4) = CalItNewton(dTime,m_dNowMJD,4);
    m_stNowPos(5) = CalItNewton(dTime,m_dNowMJD,5);
    ScenePos tmpPos;
    tmpPos.fX = m_stNowPos(0);
    tmpPos.fY = m_stNowPos(1);
    tmpPos.fZ = m_stNowPos(2);

    Math::CVector pos = m_stNowPos.slice(0,2);
    Math::CVector vPos = m_stNowPos.slice(3,5);

    for(auto one:m_vSensor)
    {
        one->SetDistance(pos.Length());
    }
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

    /// 更新卫星的位置和旋转矩阵
    m_pSatellite->SetPos(tmpPos);
    m_pSatellite->SetAttitude(rotate);
}

void CSatelliteShow::OribitColorChanged()
{
    m_pOribit->SetColor(m_sColor);
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


/// 创建节点
ISatellite *CreateNode(ISceneGraph *pSceneGraph, const string &sInterfaceName)
{
    if(sInterfaceName == S_3DSatllite)
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
    sInterfaceName = S_3DSatllite;
    return(true);
}
