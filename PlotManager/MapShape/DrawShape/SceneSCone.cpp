#include "SceneSCone.h"

CSceneSCone::CSceneSCone(ISceneGraph *pSceneGraph):
    QtSensor<ISConeSensor>(pSceneGraph)
{
}

void CSceneSCone::InitSceneNode()
{
    QtSensor<ISConeSensor>::InitSceneNode();
    BuildGeometry();
}

void CSceneSCone::SetHAngle(double dHAngle)
{
}

void CSceneSCone::SetVAngle(double dVAngle)
{
}

void CSceneSCone::BuildGeometry()
{
}
