#ifndef CMAP_SHAPE_H
#define CMAP_SHAPE_H
#include <string>
#include <NoQt.h>
#include <SceneGraph/ISceneGraph.h>
#include <Plot/MapShape/IMapPoint.h>

using namespace std;

extern "C"
{
    Q_DECL_EXPORT ISceneNode* CreateNode(ISceneGraph*pSceneGraph,const string& sInterfaceName);
    Q_DECL_EXPORT bool QueryInterface(string& sInterfaceName);
}

#endif // CMAP_SHAPE_H
