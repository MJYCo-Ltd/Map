#ifndef CMAP_SHAPE_H
#define CMAP_SHAPE_H

#include <Map/Plot/MapShape/IMapPoint.h>
#include "MapShape_Global.h"

class CMapShape
{
public:
    CMapShape();

    /**
     * @brief 返回
     * @return
     */
    static string GetInterFaceName();
private:
     string s_sPoint;
     string s_sLine;
     string s_sEllipse;
     string s_sArc;
     string s_sRectangle;
     string s_sPolygon;
     string s_sCone;
     string s_sImage;
};

extern "C"
{
    MAPSHAPESHARED_EXPORT IMapSceneNode* CreateNode(ISceneGraph*pSceneGraph,const string& sInterfaceName);
    MAPSHAPESHARED_EXPORT bool QueryInterface(string& sInterfaceName);
}

#endif // CMAP_SHAPE_H
