#ifndef QT_OSGEARTH_MAP_SCENE_NODE_H
#define QT_OSGEARTH_MAP_SCENE_NODE_H

#include <osgEarth/MapNode>
#include "IOsgEarthMapSceneNode.h"
#include "QtOsgSceneNode.h"

template <typename T>
class QtOsgEarthMapSceneNode:public QtOsgSceneNode<T>,public IOsgEarthMapSceneNode
{
public:
    explicit QtOsgEarthMapSceneNode(ISceneGraph* pSceneGraph);
    virtual ~QtOsgEarthMapSceneNode();

    /**
     * @brief  更新地图节点
     * @param pMapNode
     */
    virtual void UpdateMapNode(osgEarth::MapNode* pMapNode);
};

#include "QtOsgEarthMapSceneNode.cpp"

#endif // QT_OSGEARTH_MAP_SCENE_NODE_H
