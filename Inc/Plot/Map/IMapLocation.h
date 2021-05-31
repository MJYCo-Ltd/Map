#ifndef INTERFACE_MAP_LOCATION_HEARDER_H
#define INTERFACE_MAP_LOCATION_HEARDER_H

#include <Plot/Map/IMapSceneNode.h>
/**
 * @brief 地图位置定位模型
 */
class IMapLocation:public IMapSceneNode
{
public:

    CONSTRUCTOR(IMapLocation,IMapSceneNode)

    /**
     * @brief 设置点的位置
     * @param rGeoPos
     */
    void SetGeoPos(const ScenePos& rGeoPos)JUDGE_EQUAL_CALL_FUNCTION(rGeoPos,m_stGeoPos,PosChanged)
    const ScenePos& GeoPos(){return(m_stGeoPos);}

    /**
     * @brief 设置由SceneShpae创建的节点
     * @param pNode
     */
    virtual void SetSceneNode(ISceneNode* pNode)=0;
    virtual ISceneNode* SceneNode(){return(m_pSceneNode);}

protected:
    virtual ~IMapLocation(){}

    /**
     * @brief PosChanged
     */
    virtual void PosChanged()=0;
protected:
    ISceneNode* m_pSceneNode{};
    ScenePos  m_stGeoPos;
};

#endif//INTERFACE_MAP_LOCATION_HEARDER_H
