#include <Map/IMap.h>
#include "Satellite3D.h"
#include "Satellite2D.h"
#include "SatelliteShow.h"

string CSatelliteShow::S_sInterFace("ISatellite");

CSatelliteShow::CSatelliteShow(ISceneGraph *pSceneGrap):
    QtOsgEarthMapSceneNode<ISatellite>(pSceneGrap)
{
    m_p3D = new CSatellite3D(pSceneGrap);
}

void CSatelliteShow::InitSceneNode()
{
    m_p3D->BuildName();
}

void CSatelliteShow::SetName(const string & satName)
{
    if(m_sName != satName)
    {
        m_sName = satName;
        m_p3D->SetSatName(m_sName);
    }
}

void CSatelliteShow::SetModelPath(const string &sModelPath)
{
    if(m_sModelPath != sModelPath)
    {
        m_sModelPath = sModelPath;
        m_p3D->SetModelPath(m_sModelPath);
    }
}

void CSatelliteShow::SetOribitColor(const SceneColor &)
{
}

void CSatelliteShow::SetScalBit(double)
{
}

/// 地图更新
void CSatelliteShow::UpdateMapNode(osgEarth::MapNode *pMapNode)
{
    if(pMapNode->isGeocentric() /*&& m_pSceneGraph->GetMap()->GetSpaceEnv()*/)
    {
    }
}

/// 设置卫星轨道
void CSatelliteShow::SetJ2000Oribit(const vector<double> &vTime, const vector<Math::CVector> &rOribitInfo)
{
}

void CSatelliteShow::SetGeoOribit(const vector<Math::CVector> &vOribitInfo)
{
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
