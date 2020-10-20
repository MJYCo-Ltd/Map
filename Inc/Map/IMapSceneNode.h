#ifndef INTERFACE_MAPSCENENODE_HEADER_H
#define INTERFACE_MAPSCENENODE_HEADER_H
#include <SceneGraph/ISceneNode.h>
#include <MAP/MapType.h>

/**
 * @brief 地图节点
 */
class IMapSceneNode:public ISceneNode
{
public:
    CONSTRUCTOR(IMapSceneNode,ISceneNode)

    /**
     * @brief 判断是否被选中
     * @param unID
     * @return
     */
    virtual void JudgeSelected(unsigned unID){if(m_bSelected != (unID == m_unID)){ m_bSelected = !m_bSelected;SelecteChanged();}}
    virtual void JudgeInPolygon(const MapPolygon&)=0;
    virtual bool BeSelected()const{return(m_bSelected);}

    /**
     * @brief 转换成MapSceneNode
     * @return
     */
    virtual IMapSceneNode* AsMapSceneNode(){return(this);}

    /**
     * @brief 设置贴地模式
     */
    virtual void SetTerrainType(MAP_TERRAIN type)JUDGE_EQUAL_CALL_FUNCTION(type,m_emType,TerrainTypeChanged)
    MAP_TERRAIN TerrainType()const{return(m_emType);}
protected:
    virtual ~IMapSceneNode(){}

    /**
     * @brief 状态修改
     */
    virtual void TerrainTypeChanged()=0;
    virtual void SelecteChanged()=0;

protected:
    MAP_TERRAIN m_emType=RELATIVE_TERRAIN;
    bool        m_bSelected=false;
    unsigned    m_unID=0u;
};

#endif
