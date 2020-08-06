#ifndef QT_OSGEARTH_MAP_SCENE_NODE_H
#define QT_OSGEARTH_MAP_SCENE_NODE_H

#include <osgEarth/MapNode>
#include "IOsgEarthMapSceneNode.h"
#include "QtOsgSceneNode.h"
#include "OsgExtern/OsgExtern_Global.h"

template <typename T>
class QtOsgEarthMapSceneNode:public QtOsgSceneNode<T>,public IOsgEarthMapSceneNode
{
public:
    explicit QtOsgEarthMapSceneNode(ISceneGraph* pSceneGraph):QtOsgSceneNode<T>(pSceneGraph){}
    virtual ~QtOsgEarthMapSceneNode(){}

    /**
     * @brief  更新地图节点
     * @param pMapNode
     */
    virtual void UpdateMapNode(osgEarth::MapNode* pMapNode){}

    /**
     * @brief 是否可以删除
     * @return
     */
    virtual bool CanDelete()
    {
        return(m_bCanDelete);
    }
protected:
    bool   m_bCanDelete=true;
};

#endif // QT_OSGEARTH_MAP_SCENE_NODE_H
