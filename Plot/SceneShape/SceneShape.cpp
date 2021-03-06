#include "DrawShape/SceneCone.h"
#include "DrawShape/SceneSCone.h"
#include "DrawShape/ScenePoint.h"
#include "DrawShape/SceneLine.h"
#include "DrawShape/SceneEllipsoid.h"
#include "DrawShape/ScenePolygon.h"
#include "DrawShape/SceneImage.h"
#include "DrawShape/SceneLabel.h"
#include "SceneShape.h"

static const char s_sPoint[]="IPoint";
static const char s_sLine[]="ILine";
static const char s_sArc[]="IArc";
static const char s_sRectangle[]="IRectangle";
static const char s_sPolygon[]="IPolygon";
static const char s_sCone[]="ICone";
static const char s_sImage[]="IImage";
static const char s_sConeSensor[]="IConeSensor";
static const char s_sSConeSensor[]="ISConeSensor";
static const char s_sEllipsoidSensor[]="IEllipsoidSensor";
static const char s_sLabel[]="ILabel";

ISceneNode* CreateNode(ISceneGraph*pSceneGraph,const std::string& sInterfaceName)
{
    if(sInterfaceName == s_sConeSensor)
    {
        return(new CSceneCone(pSceneGraph));
    }
    else if(sInterfaceName == s_sSConeSensor)
    {
        return(new CSceneSCone(pSceneGraph));
    }
    else if(sInterfaceName == s_sPoint)
    {
        return(new CScenePoint(pSceneGraph));
    }
    else if(sInterfaceName == s_sLine)
    {
        return(new CSceneLine(pSceneGraph));
    }
    else if(sInterfaceName == s_sEllipsoidSensor)
    {
        return(new CSceneEllipsoid(pSceneGraph));
    }
    else if(s_sPolygon == sInterfaceName)
    {
        return(new CScenePolygon(pSceneGraph));
    }
    else if(s_sImage == sInterfaceName)
    {
        return(new CSceneImage(pSceneGraph));
    }
    else if(s_sLabel == sInterfaceName)
    {
        return(new CSceneLabel(pSceneGraph));
    }

    return(nullptr);
}

bool QueryInterface(std::string& sInterfaceName)
{
    sInterfaceName.clear();

    sInterfaceName += s_sPoint;
    sInterfaceName += " ";
    sInterfaceName += s_sLine;
    sInterfaceName += " ";
    sInterfaceName += s_sArc;
    sInterfaceName += " ";
    sInterfaceName += s_sRectangle;
    sInterfaceName += " ";
    sInterfaceName += s_sPolygon;
    sInterfaceName += " ";
    sInterfaceName += s_sCone;
    sInterfaceName += " ";
    sInterfaceName += s_sImage;
    sInterfaceName += " ";
    sInterfaceName += s_sConeSensor;
    sInterfaceName += " ";
    sInterfaceName += s_sSConeSensor;
    sInterfaceName += " ";
    sInterfaceName += s_sEllipsoidSensor;
    sInterfaceName += " ";
    sInterfaceName += s_sLabel;

    return(true);
}
