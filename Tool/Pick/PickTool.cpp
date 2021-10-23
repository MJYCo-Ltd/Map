#include "PickTool.h"
#include "PointPick.h"
#include "IntersectionPick.h"
IToolBase *CreateTool(ISceneGraph *pSceneGraph,
                      CToolSelector *pToolSelector,
                      const std::string &sInterfaceName)
{
    if(sInterfaceName == "IPointPick")
        return(new CPointPick(pSceneGraph,pToolSelector));
    else if(sInterfaceName == "IntersectionPick")
        return(new IntersectionPick(pSceneGraph,pToolSelector));
}

bool QueryInterface(std::string& sInterfaceName)
{
    sInterfaceName = "IPointPick";
    sInterfaceName += " ";
    sInterfaceName += "IntersectionPick";
    return(false);
}
