#ifndef CSCENE_SHAPE_H
#define CSCENE_SHAPE_H
#include <string>
#include <SceneGraph/ISceneGraph.h>
#include <NoQt.h>

extern "C"
{
    Q_DECL_EXPORT ISceneNode* CreateNode(ISceneGraph*pSceneGraph,const std::string& sInterfaceName);
    Q_DECL_EXPORT bool QueryInterface(std::string& sInterfaceName);
}

#endif // CSCENE_SHAPE_H
