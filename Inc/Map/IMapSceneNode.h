#ifndef INTERFACE_MAPSCENENODE_HEADER_H
#define INTERFACE_MAPSCENENODE_HEADER_H

#include "SceneGraph/ISceneNode.h"

namespace osgEarth{
    class MapNode;
}

class IMapSceneNode:public ISceneNode
{
public:
    virtual ~IMapSceneNode(){}

    /**
     * @brief 更新地图
     * @param pMapNode
     */
    virtual void UpdateMapNode(osgEarth::MapNode* pMapNode)=0;
};

#endif
