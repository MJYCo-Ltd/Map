#include "HudShape.h"
#include "HudText.h"
#include "HudImage.h"
static const char s_sHudImage[]="IHudImage";
static const char s_sHudText[]="IHudText";

ISceneNode* CreateNode(ISceneGraph*pSceneGraph,const std::string& sInterfaceName)
{
    if(sInterfaceName == s_sHudText)
    {
        return(new CHudText(pSceneGraph));
    }
    else if(sInterfaceName == s_sHudImage)
    {
        return(new CHudImage(pSceneGraph));
    }
    return(nullptr);
}

bool QueryInterface(std::string& sInterfaceName)
{
    sInterfaceName.clear();

    sInterfaceName += s_sHudImage;
    sInterfaceName += " ";
    sInterfaceName += s_sHudText;

    return(true);
}
