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
                        new RemoveTerrainCullbackScene(IOsgMapSceneNode::s_mapTerrain[T::m_pSceneGraph].get(),
                                                       m_pTerrainCallback.get()));
        }
    }

    void InitNode() override
    {
        ImplSceneNode<T>::InitNode();

        /// 需要增加到
        ImplSceneNode<T>::NodeChanged();
    }

    /**
     * @brief 设置地图节点
     * @param pNode
     */
    void SetMapSceneNode(osg::Node* pNode)
    {
        osg::ref_ptr<osgEarth::HorizonCullCallback>& pHorizonCullBack = IOsgMapSceneNode::s_mapHorizonCullBack[T::m_pSceneGraph];
        if(!pHorizonCullBack.valid())
        {
            pHorizonCullBack = new osgEarth::HorizonCullCallback;
        }

        if(IOsgSceneNode::m_pRootNode.valid())
        {
            IOsgSceneNode::m_pRootNode->removeCullCallback(pHorizonCullBack);
        }

        if(nullptr != pNode)
        {
            pNode->addCullCallback(pHorizonCullBack);
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
            IOsgMapSceneNode::s_mapTerrain[T::m_pSceneGraph]->addTerrainCallback(m_pTerrainCallback);
            m_bInsertIntoTerrain=true;
        }
        ImplSceneNode<T>::FrameCall();
    }

protected:
    bool m_bInsertIntoTerrain{false};
    bool m_bTerrainTypeChanged{false}; /// 贴地模式是否修改
    osg::ref_ptr<MayTerrainCallback>            m_pTerrainCallback;
};
#endif // IMPL_OSG_MAP_SHAPE_H
