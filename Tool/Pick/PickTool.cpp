#include "PickTool.h"
#include "PointPick.h"

IToolBase *CreateTool(ISceneGraph *pSceneGraph,
                      CToolSelector *pToolSelector,
                      const std::string &sInterfaceName)
{
    return(new CPointPick(pSceneGraph,pToolSelector));
}

bool QueryInterface(std::string& sInterfaceName)
{
    sInterfaceName = "IPointPick";
    return(false);
}
