#include "HudShape.h"
#include "HudText.h"
#include "HudImage.h"
#include "HudLayout.h"
static const char s_sHudImage[]="IHudImage";
static const char s_sHudText[]="IHudText";
static const char s_sHudLayout[]="IHudLayout";

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
    else if(sInterfaceName == s_sHudLayout)
    {
        return(new CHudLayout(pSceneGraph));
    }
    return(nullptr);
}

bool QueryInterface(std::string& sInterfaceName)
{
    sInterfaceName.clear();

    sInterfaceName += s_sHudImage;
    sInterfaceName += " ";
    sInterfaceName += s_sHudText;
    sInterfaceName += " ";
    sInterfaceName += s_sHudLayout;

    return(true);
}
