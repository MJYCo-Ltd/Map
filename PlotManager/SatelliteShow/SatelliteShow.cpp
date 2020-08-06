#include <Map/IMap.h>
#include "Satellite3D.h"
#include "Satellite2D.h"
#include "SatelliteShow.h"

string CSatelliteShow::S_sInterFace("ISatellite");

CSatelliteShow::CSatelliteShow(ISceneGraph *pSceneGrap):
    QtOsgEarthMapSceneNode<ISatellite>(pSceneGrap)
{
}

/// 地图更新
void CSatelliteShow::UpdateMapNode(osgEarth::MapNode *pMapNode)
{
    if(pMapNode->isGeocentric() && m_pSceneGraph->GetMap()->GetSpaceEnv())
    {
    }
}

void CSatelliteShow::SetJ2000Oribit(const vector<double> &vTime, const vector<Math::CVector> &rOribitInfo)
{
}

void CSatelliteShow::SetGeoOribit(const vector<Math::CVector> &vOribitInfo)
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
