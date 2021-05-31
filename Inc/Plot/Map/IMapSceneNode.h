#ifndef INTERFACE_MAPSCENENODE_HEADER_H
#define INTERFACE_MAPSCENENODE_HEADER_H

#include <SceneGraph/ISceneNode.h>

/**
 * @brief 地图节点
 */
class IMapSceneNode:public ISceneNode
{
public:
    CONSTRUCTOR(IMapSceneNode,ISceneNode)
    /**
     * @brief 地图节点高程格式
     */
    enum MAP_TERRAIN
    {
        CLOSE_TERRAIN,    /// 贴地
        RELATIVE_TERRAIN, /// 相对高程高度
        ABSOLUTE_TERRAIN  /// 相对椭球表面高度
    };

    /**
     * @brief 设置贴地模式
     */
    void SetTerrainType(MAP_TERRAIN type)JUDGE_EQUAL_CALL_FUNCTION(type,m_emType,TerrainTypeChanged)
    MAP_TERRAIN TerrainType()const{return(m_emType);}

    IMapSceneNode* AsMapSceneNode(){return(this);}
protected:
    virtual ~IMapSceneNode(){}

    /**
     * @brief 状态修改
     */
    virtual void TerrainTypeChanged()=0;

protected:
    MAP_TERRAIN m_emType{CLOSE_TERRAIN};
};

#endif
