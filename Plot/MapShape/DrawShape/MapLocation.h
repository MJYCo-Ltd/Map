#ifndef CMAPLOCATION_H
#define CMAPLOCATION_H
#include <osgEarth/Horizon>
#include <osgEarth/GeoPositionNode>
#include <Plot/Map/IMapLocation.h>
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

    virtual void Update();
    /**
     * @brief 更新地图节点
     * @param pMapNode
     */
    void UpdateMapNode();

protected:
    /**
     * @brief UpdateNode
     */
    void FrameCall();

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

    /**
     * @brief 重新计算矩阵
     */
    void ReCalculatMatrix();

protected:
    bool                                   m_bPosChanged{false};
    osgEarth::GeoPoint                     m_pGeoPoint;
    osg::observer_ptr<CMyMatrixTransform>  m_pGeoTransform;
};

#endif // CMAPLOCATION_H
