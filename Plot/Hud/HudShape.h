#ifndef C_HUD_H
#define C_HUD_H

#include <string>

#include <SceneGraph/ISceneGraph.h>
#include <NoQt.h>

extern "C"
{
    Q_DECL_EXPORT ISceneNode* CreateNode(ISceneGraph*pSceneGraph,const std::string& sInterfaceName);
    Q_DECL_EXPORT bool QueryInterface(std::string& sInterfaceName);
}

#endif // C_HUD_H
