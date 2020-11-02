#ifndef CSCENE_SHAPE_H
#define CSCENE_SHAPE_H

#include <string>
using namespace std;
#include <SceneGraph/ISceneGraph.h>
#include <NoQt.h>

extern "C"
{
    Q_DECL_EXPORT ISceneNode* CreateNode(ISceneGraph*pSceneGraph,const string& sInterfaceName);
    Q_DECL_EXPORT bool QueryInterface(string& sInterfaceName);
}

#endif // CSCENE_SHAPE_H
