#ifndef INTERFACE_OSG_MAP_SCENE_NODE_H
#define INTERFACE_OSG_MAP_SCENE_NODE_H

namespace osgEarth{
    class MapNode;
}

class IOsgMapSceneNode
{
public:
    /**
     * @brief 更新地图
     * @param pMapNode
     */
    virtual void UpdateMapNode(osgEarth::MapNode* pMapNode)=0;
};
#endif //INTERFACE_OSG_MAP_SCENE_NODE_H
