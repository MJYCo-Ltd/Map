#ifndef INTERFACE_MAPSCENENODE_HEADER_H
#define INTERFACE_MAPSCENENODE_HEADER_H

#include "SceneGraph/ISceneNode.h"

class IMapSceneNode:public ISceneNode
{
public:
    virtual ~IMapSceneNode(){}

    /**
     * @brief 是否可以删除
     * @return
     */
    virtual bool CanDelete()=0;
};

#endif
