#include "MapShape.h"

CMapShape::CMapShape()
{
}

IMapSceneNode* CreateNode(ISceneGraph*pSceneGraph,const string& sInterfaceName)
{
    return(nullptr);
}

bool QueryInterface(string& sInterfaceName)
{
    sInterfaceName.clear();

    sInterfaceName += "Point;";
    sInterfaceName += "Line;";
    sInterfaceName += "Ellipse;";
    sInterfaceName += "Arc;";
    sInterfaceName += "Rectangle;";
    sInterfaceName += "Polygon;";
    sInterfaceName += "Cone;";
    sInterfaceName += "Image";

    return(true);
}
