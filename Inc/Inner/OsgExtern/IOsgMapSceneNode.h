#ifndef INTERFACE_OSG_MAP_SCENE_NODE_H
#define INTERFACE_OSG_MAP_SCENE_NODE_H
#include <Inner/OsgExtern/OsgExtern_Global.h>
#include <osgEarth/MapNode>
#include <osgEarth/Terrain>
#include <osgEarth/Horizon>

class ISceneGraph;

class OSGEXTERNSHARED_EXPORT IOsgMapSceneNode
{
public:
    /**
     * @brief 设置静态变量
     * @param pMapNode
     */
    static void SetMapNode(osgEarth::MapNode* pMapNode,ISceneGraph* pSceneGraph)
    {
        s_mapMapNode[pSceneGraph] = pMapNode;
        s_mapIs3D[pSceneGraph] = pMapNode->isGeocentric();
        s_mapMapSRS[pSceneGraph] = pMapNode->getMapSRS();
        s_mapTerrain[pSceneGraph] = pMapNode->getTerrain();

        osg::ref_ptr<osgEarth::HorizonCullCallback>& pHorizonCullBack = s_mapHorizonCullBack[pSceneGraph];
        if(!pHorizonCullBack.valid())
        {
            pHorizonCullBack = new osgEarth::HorizonCullCallback;
        }
        pHorizonCullBack->setEnabled(pMapNode->isGeocentric());
    }

    /**
     * @brief 更新地图
     * @param pMapNode
     */
    virtual void UpdateMapNode(){}

    /**
     * @brief 瓦片数据更新
     * @param key
     */
    virtual void TileDataChanged(const osgEarth::TileKey&){}

    struct MayTerrainCallback : public osgEarth::TerrainCallback
    {
        MayTerrainCallback(){}
        void onTileUpdate(const osgEarth::TileKey& key, osg::Node*, osgEarth::TerrainCallbackContext&)
        {
            if(nullptr != m_pMapSceneNode)
            {
                m_pMapSceneNode->TileDataChanged(key);
            }
        }
        IOsgMapSceneNode* m_pMapSceneNode;
    };
protected:
    static std::map<ISceneGraph*,bool> s_mapIs3D;
    static std::map<ISceneGraph*,osg::observer_ptr<osgEarth::MapNode>> s_mapMapNode;
    static std::map<ISceneGraph*,osg::observer_ptr<const osgEarth::SpatialReference>> s_mapMapSRS;
    static std::map<ISceneGraph*,osg::observer_ptr<osgEarth::Terrain>>          s_mapTerrain;
    static std::map<ISceneGraph*,osg::ref_ptr<osgEarth::HorizonCullCallback>> s_mapHorizonCullBack;
public:
    static osg::ref_ptr<osgEarth::SpatialReference>       s_pWGS84;
};
#endif //INTERFACE_OSG_MAP_SCENE_NODE_H
