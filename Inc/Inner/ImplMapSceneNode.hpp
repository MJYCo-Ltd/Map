#ifndef IMPL_OSG_MAP_SCENE_NODE_H
#define IMPL_OSG_MAP_SCENE_NODE_H
#include <osgEarth/Style>
#include <Inner/ImplSceneNode.hpp>
#include <Inner/IOsgMapSceneNode.h>

template <typename T>
class ImplMapSceneNode:public ImplSceneNode<T>,public IOsgMapSceneNode
{
public:
    CONSTRUCTOR(ImplMapSceneNode,ImplSceneNode<T>)

    /**
     * @brief 判断地图节点是否在多边形内
     */
    void JudgeInPolygon(const MapPolygon&){}

    /**
     * @brief  更新地图节点
     * @param pMapNode
     */
    virtual void UpdateMapNode(osgEarth::MapNode* pMapNode){}

protected:

    /**
     * @brief 初始化style
     */
    void InitStyle(osgEarth::Style* pStyle)
    {
        m_pNodeStyle->getOrCreate<osgEarth::AltitudeSymbol>()
                ->clamping() = osgEarth::AltitudeSymbol::CLAMP_TO_TERRAIN;
    }

    /**
     * @brief 选中更改
     */
    void SelecteChanged(){}

    /**
     * @brief 贴地类型修改
     */
    void TerrainTypeChanged()
    {
        if(nullptr != m_pNodeStyle)
        {
            switch (T::m_emType)
            {
            case CLOSE_TERRAIN:
                m_pNodeStyle->getOrCreate<osgEarth::AltitudeSymbol>()
                        ->clamping()=osgEarth::AltitudeSymbol::CLAMP_TO_TERRAIN;
                break;
            case RELATIVE_TERRAIN:
                m_pNodeStyle->getOrCreate<osgEarth::AltitudeSymbol>()
                        ->clamping()=osgEarth::AltitudeSymbol::CLAMP_RELATIVE_TO_TERRAIN;
                break;
            }
        }
    }

protected:
    osgEarth::Style*   m_pNodeStyle=nullptr;  /// 节点样式
};

#endif // IMPL_OSG_MAP_SCENE_NODE_H
