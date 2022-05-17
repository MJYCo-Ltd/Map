#ifndef INTERFACE_MAP_GROUP_HEARDER_H
#define INTERFACE_MAP_GROUP_HEARDER_H

#include <Plot/Map/IMapSceneNode.h>
/**
 * @brief 地图位置定位模型
 */
class IMapGroup:public IMapSceneNode
{
public:

    CONSTRUCTOR(IMapGroup,IMapSceneNode)

    /**
     * @brief 设置由SceneShpae创建的节点
     * @param pNode
     */
    virtual void AddSceneNode(ISceneNode* pNode)=0;
    virtual void RemoveSceneNode(ISceneNode* pNode)=0;
    virtual ISceneNode* SceneNode(){return(m_pSceneNode);}

    virtual void Update() =0;
protected:
    virtual ~IMapGroup(){}


protected:
    ISceneNode* m_pSceneNode{};
};

#endif//INTERFACE_MAP_LOCATION_HEARDER_H
