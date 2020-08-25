#include "DrawShape/ScenePoint.h"
#include "DrawShape/SceneLine.h"
#include "DrawShape/ScenePolygon.h"
#include "DrawShape/SceneModel.h"
#include "DrawShape/SceneCone.h"
#include "DrawShape/SceneSCone.h"
#include "DrawShape/ScenePulse.h"
#include "MapShape.h"

static string s_sPoint("IPoint");
static string s_sLine("ILine");
static string s_sEllipse("IEllipse");
static string s_sArc("IArc");
static string s_sRectangle("IRectangle");
static string s_sPolygon("IPolygon");
static string s_sCone("ICone");
static string s_sImage("IImage");
static string s_sModel("IModel");
static string s_sConeSensor("IConeSensor");
static string s_sSConeSensor("ISConeSensor");
static string s_sPulseSensor("IPulseSensor");

CMapShape::CMapShape()
{
}

string CMapShape::GetInterFaceName()
{
    string sTemp;


    return(sTemp);
}

IMapSceneNode* CreateNode(ISceneGraph*pSceneGraph,const string& sInterfaceName)
{
    if(sInterfaceName == s_sPoint)
    {
        return(new CScenePoint(pSceneGraph));
    }
    else if(sInterfaceName == s_sLine)
    {
        return(new CSceneLine(pSceneGraph));
    }
    else if(sInterfaceName == s_sPolygon)
    {
        return(new CScenePolygon(pSceneGraph));
    }
    else if(sInterfaceName == s_sModel)
    {
        return(new CSceneModel(pSceneGraph));
    }
    else if(sInterfaceName == s_sConeSensor)
    {
        return(new CSceneCone(pSceneGraph));
    }
    else if(sInterfaceName == s_sSConeSensor)
    {
        return(new CSceneSCone(pSceneGraph));
    }
    else if (sInterfaceName == s_sPulseSensor)
    {
        return(new CScenePulse(pSceneGraph));
    }

    return(nullptr);
}

bool QueryInterface(string& sInterfaceName)
{
    sInterfaceName.clear();

    sInterfaceName += s_sPoint;
    sInterfaceName += " ";
    sInterfaceName += s_sLine;
    sInterfaceName += " ";
    sInterfaceName += s_sEllipse;
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
    sInterfaceName += s_sModel;
    sInterfaceName += " ";
    sInterfaceName += s_sConeSensor;
    sInterfaceName += " ";
    sInterfaceName += s_sSConeSensor;
    sInterfaceName += " ";
    sInterfaceName += s_sPulseSensor;

    return(true);
}
