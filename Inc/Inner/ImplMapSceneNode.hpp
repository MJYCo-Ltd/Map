#ifndef IMPL_OSG_MAP_SCENE_NODE_H
#define IMPL_OSG_MAP_SCENE_NODE_H
#include <Inner/ImplSceneNode.hpp>
#include <Inner/OsgExtern/IOsgMapSceneNode.h>

template <typename T>
class ImplMapSceneNode:public ImplSceneNode<T>,public IOsgMapSceneNode
{
public:
    CONSTRUCTOR(ImplMapSceneNode,ImplSceneNode<T>)

protected:
    /**
      * @brief 贴地类型修改
      */
    void TerrainTypeChanged()SET_TRUE_NODE_UPDATE(m_bTerrainTypeChanged)

    /**
     * @brief 地图节点更新
     */
    void UpdateMapNode() override
    {
        m_bMapNodeChanged=true;
        TerrainTypeChanged();
    }

    /**
      * @brief UpdateTrerrain
      */
    virtual void UpdateTrerrain(bool){}

    /**
     * @brief 重写回调
     */
    void UpdateNode()
    {
        if(m_bTerrainTypeChanged)
        {
            UpdateTrerrain(m_bMapNodeChanged);
            m_bMapNodeChanged=false;
            m_bTerrainTypeChanged=false;
        }
        ImplSceneNode<T>::UpdateNode();
    }

protected:
    bool m_bTerrainTypeChanged=false; /// 贴地模式是否修改
    bool m_bMapNodeChanged=false;
};
#endif // IMPL_OSG_MAP_SHAPE_H
