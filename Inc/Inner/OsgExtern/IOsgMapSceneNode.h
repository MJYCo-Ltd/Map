#ifndef INTERFACE_OSG_MAP_SCENE_NODE_H
#define INTERFACE_OSG_MAP_SCENE_NODE_H
#include <Inner/OsgExtern/OsgExtern_Global.h>
#include <osgEarth/MapNode>
#include <osgEarth/Terrain>

class OSGEXTERNSHARED_EXPORT IOsgMapSceneNode
{
public:
    /**
     * @brief 设置静态变量
     * @param pMapNode
     */
    static void SetMapNode(osgEarth::MapNode* pMapNode)
    {
        s_pMapNode = pMapNode;
        s_pMapSRS = pMapNode->getMapSRS();
        s_pTerrain = s_pMapNode->getTerrain();
    }

    /**
     * @brief 更新地图
     * @param pMapNode
     */
    virtual void UpdateMapNode(){}
protected:
    static osg::observer_ptr<osgEarth::MapNode>                s_pMapNode;
    static osg::observer_ptr<const osgEarth::SpatialReference> s_pMapSRS;
    static osg::observer_ptr<const osgEarth::Terrain>          s_pTerrain;
    static osg::ref_ptr<osgEarth::SpatialReference>            s_pWGS84;
};
#endif //INTERFACE_OSG_MAP_SCENE_NODE_H
