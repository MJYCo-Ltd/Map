#include "PickTool.h"
#include "PointPick.h"
#include "IntersectionPick.h"

static const char s_sPointPick[]="IPointPick";
static const char s_sIntersectionPick[]="IntersectionPick";

IToolBase *CreateTool(ISceneGraph *pSceneGraph,
                      CToolSelector *pToolSelector,
                      const std::string &sInterfaceName)
{
    if(sInterfaceName == s_sPointPick)
        return(new CPointPick(pSceneGraph,pToolSelector));
    else if(sInterfaceName == s_sIntersectionPick)
        return(new IntersectionPick(pSceneGraph,pToolSelector));
}

bool QueryInterface(std::string& sInterfaceName)
{
    sInterfaceName = s_sPointPick;
    sInterfaceName += " ";
    sInterfaceName += s_sIntersectionPick;
    return(false);
}
