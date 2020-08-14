#ifndef INTERFACE_MAPSCENENODE_HEADER_H
#define INTERFACE_MAPSCENENODE_HEADER_H

#include "SceneGraph/ISceneNode.h"

enum MAP_TERRAIN
{
    CLOSE_TERRAIN,   /// 贴地
    RELATIVE_TERRAIN /// 不贴地
};

/**
 * @brief IMapSceneNode与ISceneNode的主要区别是
 *        一个带经纬度信息，一个不带
 */
class IMapSceneNode:public ISceneNode
{
public:
    virtual ~IMapSceneNode(){}

    /**
     * @brief 设置贴地模式
     */
    virtual void SetTerrainType(MAP_TERRAIN)=0;
};

#endif
