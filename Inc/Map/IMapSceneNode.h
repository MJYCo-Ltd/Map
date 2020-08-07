#ifndef INTERFACE_MAPSCENENODE_HEADER_H
#define INTERFACE_MAPSCENENODE_HEADER_H

#include "SceneGraph/ISceneNode.h"

enum MAP_TERRAIN
{
    CLOSE_TERRAIN,   /// 贴地
    RELATIVE_TERRAIN /// 不贴地
};

class IMapSceneNode:public ISceneNode
{
public:
    virtual ~IMapSceneNode(){}

    /**
     * @brief 是否可以删除
     * @return
     */
    virtual bool CanDelete() const =0;

    /**
     * @brief 设置贴地模式
     */
    virtual void SetTerrainType(MAP_TERRAIN)=0;
};

#endif
