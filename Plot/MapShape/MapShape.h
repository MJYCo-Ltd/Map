#ifndef CMAP_SHAPE_H
#define CMAP_SHAPE_H
#include <string>
#include <NoQt.h>
#include <SceneGraph/ISceneGraph.h>
#include <Plot/MapShape/IMapPoint.h>

extern "C"
{
    Q_DECL_EXPORT ISceneNode* CreateNode(ISceneGraph*pSceneGraph,const std::string& sInterfaceName);
    Q_DECL_EXPORT bool QueryInterface(std::string& sInterfaceName);
}

#endif // CMAP_SHAPE_H
