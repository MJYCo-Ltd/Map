#include "DrawShape/MapPoint.h"
#include "DrawShape/MapLine.h"
#include "DrawShape/MapPolygon.h"
#include "DrawShape/MapCoverImage.h"
#include "DrawShape/MapLocation.h"
#include "DrawShape/MapPolygon3D.h"
#include "MapShape.h"

static const char s_sPoint[]="IMapPoint";
static const char s_sLine[]="IMapLine";
static const char s_sEllipse[]="IMapEllipse";
static const char s_sArc[]="IMapArc";
static const char s_sRectangle[]="IMapRectangle";
static const char s_sPolygon[]="IMapPolygon";
static const char s_sPolygon3D[]="IMapPolygon3D";
static const char s_sCoverImage[]="IMapCoverImage";
static const char s_sLocation[]="IMapLocation";

ISceneNode *CreateNode(ISceneGraph*pSceneGraph,const std::string& sInterfaceName)
{
    if(s_sPoint == sInterfaceName)
    {
        return(new CMapPoint(pSceneGraph));
    }
    else if(s_sLine == sInterfaceName)
    {
        return(new CMapLine(pSceneGraph));
    }
    else if(s_sPolygon == sInterfaceName)
    {
        return(new CMapPolygon(pSceneGraph));
    }
    else if(s_sCoverImage == sInterfaceName)
    {
        return(new CMapCoverImage(pSceneGraph));
    }
    else if(s_sLocation == sInterfaceName)
    {
        return(new CMapLocation(pSceneGraph));
    }
    else if(s_sPolygon3D == sInterfaceName)
    {
        return(new CMapPolygon3D(pSceneGraph));
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
    sInterfaceName += s_sPolygon3D;
    sInterfaceName += " ";
    sInterfaceName += s_sCoverImage;
    sInterfaceName += " ";
    sInterfaceName += s_sLocation;


    return(true);
}
