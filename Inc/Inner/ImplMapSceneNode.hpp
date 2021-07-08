#ifndef IMPL_OSG_MAP_SCENE_NODE_H
#define IMPL_OSG_MAP_SCENE_NODE_H
#include <osgEarth/Horizon>
#include <Inner/ImplSceneNode.hpp>
#include <Inner/OsgExtern/IOsgMapSceneNode.h>

class RemoveTerrainCullbackScene:public osg::Operation
{
public:
    RemoveTerrainCullbackScene(osgEarth::Terrain* pTerrain,
                               osgEarth::TerrainCallback* pTerrainCallback):
        m_pTerrain(pTerrain),
        m_pTerrainCallback(pTerrainCallback){}

    /**
     * @brief 回调
     */
    void operator()(osg::Object*)
    {
        m_pTerrain->removeTerrainCallback(m_pTerrainCallback);
    }
protected:
    osg::ref_ptr<osgEarth::Terrain>         m_pTerrain;
    osg::ref_ptr<osgEarth::TerrainCallback> m_pTerrainCallback;
};

template <typename T>
class ImplMapSceneNode:public ImplSceneNode<T>,public IOsgMapSceneNode
{
public:
    CONSTRUCTOR(ImplMapSceneNode,ImplSceneNode<T>)

protected:
    ~ImplMapSceneNode()
    {
        if(m_pTerrainCallback.valid())
        {
            m_pTerrainCallback->m_pMapSceneNode=nullptr;
            T::m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(
                        new RemoveTerrainCullbackScene(s_pTerrain.get(),
                                                       m_pTerrainCallback.get()));
        }
    }

    void InitNode() override
    {
        ImplSceneNode<T>::InitNode();

        /// 增加地平面回调
        m_pHorizonCullBack = new osgEarth::HorizonCullCallback;
        m_pHorizonCullBack->setEnabled(true);

        /// 需要增加到
        ImplSceneNode<T>::NodeChanged();
    }

    void UpdateMapNode() override
    {
        m_pHorizonCullBack->setEnabled(ImplMapSceneNode<T>::s_pMapNode->isGeocentric());
    }

    /**
     * @brief 设置地图节点
     * @param pNode
     */
    void SetMapSceneNode(osg::Node* pNode)
    {
        if(IOsgSceneNode::m_pRootNode.valid())
        {
            IOsgSceneNode::m_pRootNode->removeCullCallback(m_pHorizonCullBack);
        }

        if(nullptr != pNode)
        {
            pNode->addCullCallback(m_pHorizonCullBack);
        }

        ImplSceneNode<T>::SetOsgNode(pNode);
    }

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
    void FrameCall()
    {
        if(m_bTerrainTypeChanged)
        {
            UpdateTrerrain();
            m_bTerrainTypeChanged=false;
        }

        if(!m_bInsertIntoTerrain)
        {
            m_pTerrainCallback = new MayTerrainCallback;
            m_pTerrainCallback->m_pMapSceneNode=this;
            s_pTerrain->addTerrainCallback(m_pTerrainCallback);
            m_bInsertIntoTerrain=true;
        }
        ImplSceneNode<T>::FrameCall();
    }

protected:
    bool m_bInsertIntoTerrain{false};
    bool m_bTerrainTypeChanged{false}; /// 贴地模式是否修改
    osg::ref_ptr<MayTerrainCallback>            m_pTerrainCallback;
    osg::ref_ptr<osgEarth::HorizonCullCallback> m_pHorizonCullBack;
};
#endif // IMPL_OSG_MAP_SHAPE_H
