#include "SoundTool.h"

CSoundTool::CSoundTool()
{

}

ITool *CreateTool(ISceneGraph *pSceneGraph, const std::string &sInterfaceName)
{
    return(nullptr);
}

bool QueryInterface(std::string& sInterfaceName)
{
    sInterfaceName = "ISound";
    return(false);
}
