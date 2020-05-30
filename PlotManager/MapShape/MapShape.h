#ifndef CMAP_SHAPE_H
#define CMAP_SHAPE_H

#include <Map/Plot/MapShape/IMapPoint.h>
#include "MapShape_Global.h"

class CMapShape
{
public:
    CMapShape();
};

extern "C"
{
    MAPSHAPESHARED_EXPORT IMapSceneNode* CreateNode(ISceneGraph*pSceneGraph,const string& sInterfaceName);
    MAPSHAPESHARED_EXPORT bool QueryInterface(string& sInterfaceName);
}

#endif // CMAP_SHAPE_H
