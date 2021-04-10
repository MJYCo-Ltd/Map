#ifndef CMAPLOCATION_H
#define CMAPLOCATION_H
#include <osgEarth/Horizon>
#include <osgEarth/GeoPositionNode>
#include <Plot/Map/IMapLocation.h>
#include <Inner/ImplSceneNode.hpp>
#include <Inner/IOsgMapSceneNode.h>
#include <Inner/OsgExtern/MyMatrixTransform.h>
class CMapLocation:public ImplSceneNode<IMapLocation>,public IOsgMapSceneNode
{
public:
    CONSTRUCTOR(CMapLocation,ImplSceneNode<IMapLocation>)

    /**
     * @brief
     * @param pNode
     */
    void SetSceneNode(ISceneNode* pNode);

    /**
     * @brief 更新地图节点
     * @param pMapNode
     */
    void UpdateMapNode(osgEarth::MapNode* pMapNode);

protected:
    /**
     * @brief UpdateNode
     */
    void UpdateNode();

    /**
     * @brief 初始化节点
     */
    void InitNode();

    /**
     * @brief 位置更改
     */
    void PosChanged();

    /**
     * @brief 贴地状态更改
     */
    void TerrainTypeChanged();

protected:
    osgEarth::GeoPoint                                m_pGeoPoint;
    osg::observer_ptr<const osgEarth::SpatialReference> m_pMapSrs;
    osg::observer_ptr<osgEarth::Terrain>               m_pTerrain;
    osg::observer_ptr<CMyMatrixTransform>       m_pGeoTransform;
    osg::ref_ptr<osgEarth::HorizonCullCallback> m_pHorizonCullBack;
};

#endif // CMAPLOCATION_H
