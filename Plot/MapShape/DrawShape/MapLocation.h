#ifndef CMAPLOCATION_H
#define CMAPLOCATION_H
#include <osgEarth/Horizon>
#include <osgEarth/GeoPositionNode>
#include <Plot/MapShape/IMapLocation.h>
#include <Inner/ImplMapSceneNode.hpp>
#include <Inner/OsgExtern/MyMatrixTransform.h>
class CMapLocation:public ImplMapSceneNode<IMapLocation>
{
public:
    CONSTRUCTOR(CMapLocation,ImplMapSceneNode<IMapLocation>)

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

    /**
     * @brief 初始化节点
     */
    void InitNode();

    /**
     * @brief 位置更改
     */
    void PosChanged();

protected:
    osgEarth::GeoPoint                                m_pGeoPoint;
    osg::observer_ptr<const osgEarth::SpatialReference> m_pMapSrs;
    osg::observer_ptr<osgEarth::Terrain>               m_pTerrain;
    osg::observer_ptr<CMyMatrixTransform>       m_pGeoTransform;
    osg::ref_ptr<osgEarth::HorizonCullCallback> m_pHorizonCullBack;
};

#endif // CMAPLOCATION_H