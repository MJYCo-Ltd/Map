#include "ToolSelector.h"


ITool* CreateToolSelector(ISceneGraph* pSceneGraph)
{
    return(new CToolSelector(pSceneGraph));
}

void CToolSelector::RegisterTool()
{
}
