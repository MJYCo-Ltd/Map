#include "AnalysisTool.h"

CAnalysisTool::CAnalysisTool()
{

}

ITool *CreateTool(ISceneGraph *pSceneGraph, const std::string &sInterfaceName)
{
    return(nullptr);
}

bool QueryInterface(std::string& sInterfaceName)
{
    sInterfaceName = "IViewAnalysis";
    return(false);
}
