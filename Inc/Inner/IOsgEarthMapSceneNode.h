#ifndef INTERFACE_OSGEARTH_MAP_SCENE_NODE_H
#define INTERFACE_OSGEARTH_MAP_SCENE_NODE_H

namespace osgEarth{
    class MapNode;
}

class IOsgEarthMapSceneNode
{
public:
    /**
     * @brief 更新地图
     * @param pMapNode
     */
    virtual void UpdateMapNode(osgEarth::MapNode* pMapNode)=0;
};
#endif
