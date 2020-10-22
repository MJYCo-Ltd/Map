#ifndef CSCENE_SHAPE_H
#define CSCENE_SHAPE_H

#include <SceneGraph/ISceneGraph.h>
#include <NoQt.h>

class CSceneShape
{
public:
    CSceneShape();

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

#endif // CSCENE_SHAPE_H
