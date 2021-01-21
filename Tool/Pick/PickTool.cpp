#include "PickTool.h"
#include "PointPick.h"

CPickTool::CPickTool()
{
}

IToolBase *CreateTool(ISceneGraph *pSceneGraph, const std::string &sInterfaceName)
{
    return(new CPointPick(pSceneGraph));
}

bool QueryInterface(std::string& sInterfaceName)
{
    sInterfaceName = "IPointPick";
    return(false);
}
