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
      * @brief 贴地方式更改
      */
    virtual void UpdateTrerrain(){}

    /**
     * @brief 重写回调
     */
    void UpdateNode()
    {
        if(m_bTerrainTypeChanged)
        {
            UpdateTrerrain();
            m_bTerrainTypeChanged=false;
        }
        ImplSceneNode<T>::UpdateNode();
    }

protected:
    bool m_bTerrainTypeChanged=false; /// 贴地模式是否修改
};
#endif // IMPL_OSG_MAP_SHAPE_H
