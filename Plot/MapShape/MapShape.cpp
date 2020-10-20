#include "DrawShape/MapPoint.h"
#include "DrawShape/MapLine.h"
#include "DrawShape/MapPolygon.h"
#include "DrawShape/MapCoverImage.h"
#include "DrawShape/MapLocation.h"
#include "MapShape.h"

static string s_sPoint("IMapPoint");
static string s_sLine("IMapLine");
static string s_sEllipse("IMapEllipse");
static string s_sArc("IMapArc");
static string s_sRectangle("IMapRectangle");
static string s_sPolygon("IMapPolygon");
static string s_sCoverImage("IMapCoverImage");
static string s_sLocation("IMapLocation");

CMapShape::CMapShape()
{
}

string CMapShape::GetInterFaceName()
{
    string sTemp;


    return(sTemp);
}

ISceneNode *CreateNode(ISceneGraph*pSceneGraph,const string& sInterfaceName)
{
    if(sInterfaceName == s_sPoint)
    {
        return(new CMapPoint(pSceneGraph));
    }
    else if(sInterfaceName == s_sLine)
    {
        return(new CMapLine(pSceneGraph));
    }
    else if(sInterfaceName == s_sPolygon)
    {
        return(new CMapPolygon(pSceneGraph));
    }
    else if(sInterfaceName == s_sCoverImage)
    {
        return(new CMapCoverImage(pSceneGraph));
    }
    else if(sInterfaceName == s_sLocation)
    {
        return(new CMapLocation(pSceneGraph));
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
    sInterfaceName += s_sCoverImage;
    sInterfaceName += " ";
    sInterfaceName += s_sLocation;


    return(true);
}
