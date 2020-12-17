#include "DrawShape/MapPoint.h"
#include "DrawShape/MapLine.h"
#include "DrawShape/MapPolygon.h"
#include "DrawShape/MapCoverImage.h"
#include "DrawShape/MapLocation.h"
#include "MapShape.h"

static std::string s_sPoint("IMapPoint");
static std::string s_sLine("IMapLine");
static std::string s_sEllipse("IMapEllipse");
static std::string s_sArc("IMapArc");
static std::string s_sRectangle("IMapRectangle");
static std::string s_sPolygon("IMapPolygon");
static std::string s_sCoverImage("IMapCoverImage");
static std::string s_sLocation("IMapLocation");

ISceneNode *CreateNode(ISceneGraph*pSceneGraph,const std::string& sInterfaceName)
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

bool QueryInterface(std::string& sInterfaceName)
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
