#include "HudShape.h"
#include "HudText.h"
#include "HudImage.h"
#include "HudLayout.h"
#include "HudButton.h"

static const char s_sHudImage[]="IHudImage";
static const char s_sHudText[]="IHudText";
static const char s_sHudLayout[]="IHudLayout";
static const char s_sHudVBox[]="IHudVBox";
static const char s_sHudButton[]="IHudButton";

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
    else if(sInterfaceName == s_sHudButton)
    {
        return(new CHudButton(pSceneGraph));
    }
    else if(sInterfaceName == s_sHudVBox)
    {
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
    sInterfaceName += " ";
    sInterfaceName += s_sHudButton;
    sInterfaceName += " ";
    sInterfaceName += s_sHudVBox;


    return(true);
}
