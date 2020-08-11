#ifndef QT_OSGEARTH_MAP_SCENE_NODE_H
#define QT_OSGEARTH_MAP_SCENE_NODE_H
#include <osgEarth/MapNode>
#include "IOsgEarthMapSceneNode.h"
#include "QtOsgSceneNode.h"

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
     * @brief 设置
     * @param emType
     */
    virtual void SetTerrainType(MAP_TERRAIN emType)
    {
        if(m_emTerrain != emType)
        {
            m_emTerrain = emType;
            TerrainTypeChanged();
        }
    }

    /**
     * @brief 是否可以删除
     * @return
     */
    virtual bool CanDelete() const
    {
        return(QtOsgSceneNode<T>::m_pOsgNode->referenceCount() < 2);
    }

protected:
    virtual void TerrainTypeChanged(){};
protected:
    bool   m_bCanDelete=true;
    MAP_TERRAIN m_emTerrain;
};

#endif // QT_OSGEARTH_MAP_SCENE_NODE_H
