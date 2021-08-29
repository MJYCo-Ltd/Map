#include <VersionMathCommon.h>
#include <Math/Intpol.h>
#include <Math/VecMat.h>
#include <Plot/SceneShape/ISensor.h>
#include <Plot/SceneShape/ILine.h>
#include <Plot/Common/ISceneAttitudeGroup.h>
#include <Plot/Common/ISceneScaleGroup.h>
#include <Plot/Common/ISceneLodGroup.h>
#include <Plot/SceneShape/IImage.h>
#include <Plot/IPlot.h>
#include <Plot/SceneShape/ILine.h>
#include <Plot/Common/ISceneModel.h>
#include <Plot/Map/IMap.h>
#include <Plot/Map/SpaceEnv/ISpaceEnv.h>
#include "SatelliteShow.h"

static char S_3DSatllite[]="ISatellite";

void CSatelliteShow::InitNode()
{
    m_stNowPos.Resize(6);
    m_satelliteWgs84PV.Resize(6);
    ImplSceneGroup<ISatellite>::InitNode();

	/// 加载模型
	m_pSatellite = m_pSceneGraph->GetPlot()->CreateSceneGroup(ATTITUDE_GROUP)->AsSceneAttitudeGroup();
	m_pSatelliteAtt = m_pSceneGraph->GetPlot()->CreateSceneGroup(ATTITUDE_GROUP)->AsSceneAttitudeGroup();
	m_pSatelliteScale = m_pSceneGraph->GetPlot()->CreateSceneGroup(SCALE_GROUP)->AsSceneScaleGroup();

	m_pSatelliteScale->SetAutoScal(false);
	m_pSatelliteScale->SetMinScal(1);
	//m_pSatelliteAtt->AddSceneNode(m_pSatelliteScale);
	m_pSatellite->AddSceneNode(m_pSatelliteAtt);

    m_pOribit = dynamic_cast<ILine*>(m_pSceneGraph->GetPlot()->CreateSceneNode("ILine"));
    m_pOribit->OpenGlow(true);
    AddSceneNode(m_pSatellite);
    AddSceneNode(m_pOribit);
}


/// 设置卫星轨道
void CSatelliteShow::SetJ2000Oribit(const std::vector<double> &vTime, const std::vector<Math::CVector> &rOribitInfo)
{
    m_vOribit = rOribitInfo;
    m_vdMjd = vTime;
    m_dStart = vTime[0];
    m_dEnd = vTime[vTime.size()-1];
    m_dStep = vTime[1]-vTime[0];

    std::vector<ScenePos> vTemp;
    vTemp.resize(rOribitInfo.size());
    int nIndex(0);
    for(auto iter=rOribitInfo.begin();iter!=rOribitInfo.end();++iter,++nIndex)
    {
        vTemp[nIndex].dX = iter->GetX();
        vTemp[nIndex].dY = iter->GetY();
        vTemp[nIndex].dZ = iter->GetZ();
    }
    m_pOribit->SetMultPos(vTemp);
}

void CSatelliteShow::SetECFOribit(const std::vector<Math::CVector>& vOribitInfo)
{
    m_vEcfOribit = vOribitInfo;
}

/// 增加传感器
void CSatelliteShow::AddSensor(int id, ISensor *pSensor)
{
    if(m_vSensor.find(pSensor) != m_vSensor.end())
    {
        return;
    }

    auto pAtt = m_pSceneGraph->GetPlot()->CreateSceneGroup(ATTITUDE_GROUP)->AsSceneAttitudeGroup();
    pAtt->AddSceneNode(pSensor);
    m_pSatellite->AddSceneNode(pAtt);

    m_vSensor.insert(pSensor);
    m_SensorAttMap[id] = pAtt;
}

const Math::CVector& CSatelliteShow::GetSatelliteWgs84PV() const
{
    return m_satelliteWgs84PV;
}

void CSatelliteShow::SetScale(double dScale)
{
    m_pSatelliteScale->SetScal(dScale);
}

void CSatelliteShow::SetCorrectAttitude(const SceneAttitude& rAttitude)
{
    m_satelliteCorrectAttitude = rAttitude;
    if(nullptr != m_pModel)
    {
        m_pModel->InitAttitude(m_satelliteCorrectAttitude);
    }
}

void CSatelliteShow::SetAttitude(const SceneAttitude& rAttitude)
{
    m_satelliteAttitude = rAttitude;
    m_pSatelliteAtt->SetAttitude(rAttitude);
}

const SceneAttitude &CSatelliteShow::GetAttitude() const
{
    return(m_satelliteAttitude);
}

void CSatelliteShow::SetSensorAttitude(int id, const SceneAttitude& rAttitude)
{
    if (m_SensorAttMap.find(id) != m_SensorAttMap.end())
    {
        m_SensorAttMap[id]->SetAttitude(rAttitude);
        m_SensorAttValueMap[id] = rAttitude;
    }
}

const SceneAttitude &CSatelliteShow::GetSensorAttitude(int id) const
{
    auto pFindOne = m_SensorAttValueMap.find(id);
    if (pFindOne != m_SensorAttValueMap.end())
    {
        return(pFindOne->second);
    }
    else
    {
        static SceneAttitude s_Attitude;
        return(s_Attitude);
    }
}

void CSatelliteShow::UpdateJ2000OribitShow(double duration)
{
    if (m_vdMjd.size() > 2)
    {
        double dStep = m_vdMjd[1] - m_vdMjd[0];
        double dTimeCount = 0;

        std::vector<ScenePos> vTemp;
        for (auto iter = m_vOribit.begin();iter != m_vOribit.end();++iter)
        {
            ScenePos pos;
            pos.dX = iter->GetX();
            pos.dY = iter->GetY();
            pos.dZ = iter->GetZ();
            if (dTimeCount * 86400 > duration)
                break;
            else
                vTemp.push_back(pos);
            dTimeCount += dStep;
        }
        m_pOribit->SetMultPos(vTemp);
    }
}

void CSatelliteShow::SetFont(int fontSize, SceneColor fontFillColor, SceneColor fontOutColor)
{
    m_pSatelliteName->SetFontSize(fontSize);
}

void CSatelliteShow::SetPicPath(const std::string& sPicPath)
{
    m_sPicPath = sPicPath;
}

void CSatelliteShow::SetLodDis(double dis)
{
    m_lodDis = dis;
}

/// 模型修改
void CSatelliteShow::ModelChanged()
{
    if(nullptr != m_pModel)
    {
        if(nullptr != m_pSatelliteName)
        {
            m_pSatelliteName->DisAttachNode(m_pModel);
        }

        m_pSatelliteScale->RemoveSceneNode(m_pModel);
    }

	//2D图标
	auto pImage = dynamic_cast<IImage*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IImage"));
	pImage->SetImagePath(m_sPicPath);
	pImage->OpenLight(false);
	//pImage->AlwasOnTop(true);
	auto pAutoImage = m_pSceneGraph->GetPlot()->CreateSceneGroup(SCALE_GROUP)->AsSceneScaleGroup();
	pAutoImage->SetAutoScal(true);
	pAutoImage->SetMinScal(1.);
	pAutoImage->AddSceneNode(pImage);
    //3D模型
    m_pModel = m_pSceneGraph->GetPlot()->LoadSceneNode(m_sModelPath)->AsSceneModel();
    m_pSatelliteScale->AddSceneNode(m_pModel);
    //分级显示
    auto pLod = m_pSceneGraph->GetPlot()->CreateSceneGroup(LOD_GROUP)->AsSceneLodGroup();
    pLod->AddSceneNode(pAutoImage);
    pLod->AddSceneNode(m_pSatelliteScale);
    std::vector<float> vLevelInfo;
    vLevelInfo.push_back(m_lodDis);
    pLod->SetLevelsInfo(vLevelInfo);
    m_pSatelliteAtt->AddSceneNode(pLod);

    if(nullptr == m_pSatelliteName)
    {
        m_pSatelliteName = dynamic_cast<ILabel*>(m_pSceneGraph->GetPlot()->CreateSceneNode("ILabel"));
        m_pSatelliteName->SetText(m_sName);
        m_pSatelliteName->SetFontSize(15);
        m_pSatelliteName->SetFont("fonts/msyh.ttf");
    }

    m_pSatelliteName->SetAttachNode(m_pSatelliteAtt);
}

///名称修改
void CSatelliteShow::NameChanged()
{
    if(nullptr != m_pSatelliteName)
    {
        m_pSatelliteName->SetText(m_sName);
    }
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
    //当前wgs84位置
    if (m_vEcfOribit.size() > 1)
    {
        m_satelliteWgs84PV(0) = CalItNewtonEcf(dTime, m_dNowMJD, 0);
        m_satelliteWgs84PV(1) = CalItNewtonEcf(dTime, m_dNowMJD, 1);
        m_satelliteWgs84PV(2) = CalItNewtonEcf(dTime, m_dNowMJD, 2);
        m_satelliteWgs84PV(3) = CalItNewtonEcf(dTime, m_dNowMJD, 3);
        m_satelliteWgs84PV(4) = CalItNewtonEcf(dTime, m_dNowMJD, 4);
        m_satelliteWgs84PV(5) = CalItNewtonEcf(dTime, m_dNowMJD, 5);
    }
    //当前j2000位置
    m_stNowPos(0) = CalItNewton(dTime, m_dNowMJD, 0);
    m_stNowPos(1) = CalItNewton(dTime, m_dNowMJD, 1);
    m_stNowPos(2) = CalItNewton(dTime, m_dNowMJD, 2);
    m_stNowPos(3) = CalItNewton(dTime, m_dNowMJD, 3);
    m_stNowPos(4) = CalItNewton(dTime, m_dNowMJD, 4);
    m_stNowPos(5) = CalItNewton(dTime, m_dNowMJD, 5);
    ScenePos tmpPos;
    tmpPos.dX = m_stNowPos(0);
    tmpPos.dY = m_stNowPos(1);
    tmpPos.dZ = m_stNowPos(2);

    Math::CVector pos = m_stNowPos.slice(0,2);
    Math::CVector vPos = m_stNowPos.slice(3,5);

    for(auto one:m_vSensor)
    {
        dynamic_cast<ISensor*>(one)->SetDistance(pos.Length()-6000.137e3);
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

double CSatelliteShow::CalItNewtonEcf(double* dX, double dT, int nDim)
{
    static double dY[3];
    dY[0] = m_vEcfOribit[m_nIndex - 1](nDim);
    dY[1] = m_vEcfOribit[m_nIndex](nDim);
    dY[2] = m_vEcfOribit[m_nIndex + 1](nDim);

    return(Numerical::Cntpol::ItNewton(3, dX, dY, dT));
}

/// 创建节点
ISatellite *CreateNode(ISceneGraph *pSceneGraph, const std::string &sInterfaceName)
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

bool QueryInterface(std::string& sInterfaceName)
{
    sInterfaceName = S_3DSatllite;
    return(true);
}
