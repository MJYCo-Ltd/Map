#ifndef CMAP_SHAPE_H
#define CMAP_SHAPE_H

#include <NoQt.h>
#include <SceneGraph/ISceneGraph.h>
#include <Plot/MapShape/IMapPoint.h>

class CMapShape
{
public:
    CMapShape();

    /**
     * @brief 返回
     * @return
     */
    static string GetInterFaceName();
};

extern "C"
{
    Q_DECL_EXPORT ISceneNode* CreateNode(ISceneGraph*pSceneGraph,const string& sInterfaceName);
    Q_DECL_EXPORT bool QueryInterface(string& sInterfaceName);
}

#endif // CMAP_SHAPE_H
