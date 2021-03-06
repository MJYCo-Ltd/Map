#include <osgViewer/View>
#include <osg/MatrixTransform>
#include <osgEarth/Version>
#include <osgEarth/Terrain>

#if OSGEARTH_VERSION_GREATER_OR_EQUAL(3,0,0)
#include <osgEarth/LogarithmicDepthBuffer>
#else
#include <osgEarthUtil/LogarithmicDepthBuffer>
#endif

#include <osgEarth/GLUtils>
#include <osgEarth/AutoClipPlaneHandler>

#include <Inner/IRender.h>
#include <Inner/ILoadResource.h>
#include <Inner/IOsgSceneNode.h>
#include <Inner/IOsgViewPoint.h>
#include <Inner/OsgExtern/OsgExtern.h>
#include <SceneGraph/IWindow.h>
#include <SceneGraph/IViewPort.h>
#include <Plot/Map/IMapObserver.h>
#include "MapNodeChanged.h"
#include "MapModifyLayer.h"
#include "SpaceEnv.h"
#include "Map.h"
#include "MapLayer.h"
#include "MapModelLayer.h"

/// 析构函数
CMap::~CMap()
{
    ClearLayers();

    if(m_p2DRoot.valid())
    {
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new RemoveFromeScene(m_p2DRoot));
        m_p2DRoot = nullptr;
    }
    if (m_pMap3DNode.valid())
    {
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new RemoveFromeScene(m_pMap3DNode));
        m_pMap3DNode = nullptr;
    }

    if (m_pMap2DNode.valid())
    {
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new RemoveFromeScene(m_pMap2DNode));
        m_pMap2DNode = nullptr;
    }

    /// 清空根节点
    m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new RemoveFromeScene(m_pGroup.get()));
}

/// 订阅消息
void CMap::SubMessage(IMapMessageObserver *pMsgObr)
{
    auto findResult = find(m_listObserver.begin(),m_listObserver.end(),pMsgObr);
    if(m_listObserver.end() == findResult)
    {
        m_listObserver.push_back(pMsgObr);
    }
}

/// 取消订阅
void CMap::UnSubMessage(IMapMessageObserver *pMsgObr)
{
    auto findResult = find(m_listObserver.begin(),m_listObserver.end(),pMsgObr);
    if(findResult != m_listObserver.end())
    {
        m_listObserver.erase(findResult);
    }
}

/// 坐标转换
bool CMap::ConvertCoord(int &nX, int &nY, MapGeoPos &geoPos, short TranType)
{
    if(0 == TranType)
    {
        osg::Vec3d world;
        osgEarth::GeoPoint geoPoint;
        auto pOsgViewPoint = dynamic_cast<IOsgViewPoint*>(m_pSceneGraph->GetMainWindow()->GetMainViewPoint());
        if(!pOsgViewPoint || !pOsgViewPoint->GetOsgView())
        {
            return(false);
        }

        osg::Viewport* pViewPort = pOsgViewPoint->GetOsgView()->getCamera()->getViewport();
        switch (m_emType)
        {
        case MAP_2D:
            if(m_pMap2DNode->getTerrain()->getWorldCoordsUnderMouse(pOsgViewPoint->GetOsgView(), nX, pViewPort ? pViewPort->height() - nY : nY, world))
            {
                geoPoint.fromWorld(m_pMap2DNode->getMapSRS(),world);
                geoPoint.makeGeographic();
                geoPos.fLon = geoPoint.x();
                geoPos.fLat = geoPoint.y();
                geoPos.fHeight = geoPoint.z();
                return (true);
            }
            break;
        case MAP_3D:
            if(m_pMap3DNode->getTerrain()->getWorldCoordsUnderMouse(pOsgViewPoint->GetOsgView(), nX, pViewPort ? pViewPort->height() - nY : nY, world))
            {
                geoPoint.fromWorld(m_pMap3DNode->getMapSRS(),world);
                geoPoint.makeGeographic();
                geoPos.fLon = geoPoint.x();
                geoPos.fLat = geoPoint.y();
                geoPos.fHeight = geoPoint.z();
                return (true);
            }
            break;
        }
        return(false);
    }
    else if(1==TranType)
    {
        auto pOsgViewPoint = dynamic_cast<IOsgViewPoint*>(m_pSceneGraph->GetMainWindow()->GetMainViewPoint());
        if(!pOsgViewPoint || !pOsgViewPoint->GetOsgView())
        {
            return(false);
        }

        auto pView = pOsgViewPoint->GetOsgView();

        osg::Vec3d world;
        osgEarth::GeoPoint geoPoint(osgEarth::SpatialReference::create("wgs84"),geoPos.fLon,geoPos.fLat,geoPos.fHeight),geoOut;
        switch (m_emType)
        {
        case MAP_2D:
            if(geoPoint.transform(m_pMap2DNode->getMapSRS(),geoOut) &&
               m_pMap2DNode->getMapSRS()->transformToWorld(osg::Vec3d(geoOut.x(),geoOut.y(),geoOut.z()),world))
            {
                osg::Matrixd _MVPW = pView->getCamera()->getViewMatrix() * pView->getCamera()->getProjectionMatrix()
                        * pView->getCamera()->getViewport()->computeWindowMatrix();

                osg::Vec3d scrennPos = world * _MVPW;
                nX = scrennPos.x();
                nY = pView->getCamera()->getViewport()->height() - scrennPos.y();
                return(true);
            }
            break;
        case MAP_3D:
            if(m_pMap3DNode->getMapSRS()->transformToWorld(osg::Vec3d(geoPos.fLon,geoPos.fLat,geoPos.fHeight),world))
            {
                osg::Matrixd _MVPW = pView->getCamera()->getViewMatrix() * pView->getCamera()->getProjectionMatrix()
                        * pView->getCamera()->getViewport()->computeWindowMatrix();

                osg::Vec3d scrennPos = world * _MVPW;
                nX = scrennPos.x();
                nY = pView->getCamera()->getViewport()->height() - scrennPos.y();
                return(true);
            }
            break;
        }

        return(false);
    }
    else
    {
        return(false);
    }
}

/// 获取所有的图层
MapLayers CMap::GetMapLayers() const
{
    MapLayers tmpLayers(m_earthFileLayers);
    for(auto one : m_userLayers)
    {
        tmpLayers.push_back(one.first);
    }
    return(tmpLayers);
}

/// 控制图层显隐
void CMap::SetLayerVisible(const std::string & sLayerName)
{
}

/// 创建图层
IMapLayer *CMap::CreateLayer(const std::string & sLayerName)
{
    auto findOne = m_userLayers.find(sLayerName);
    if(m_userLayers.end() != findOne)
    {
        return(findOne->second);
    }
    else
    {
        osgEarth::MapNode* pMapNode(nullptr);
        switch (m_emType)
        {
        case MAP_2D:
            pMapNode = m_pMap2DNode;
            break;
        case MAP_3D:
            pMapNode = m_pMap3DNode;
            break;
        }
        CMapLayer* pLayer = new CMapLayer(sLayerName,pMapNode,m_pSceneGraph);
        m_userLayers[sLayerName] = pLayer;
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CMapModifyLayer(pMapNode,pLayer->GetModelLayer(),true));

        /// 通知观察者
        for(auto one:m_listObserver)
        {
            one->AddLayer(sLayerName);
        }

        return (pLayer);
    }
}

/// 移除图层
bool CMap::RemoveLayer(IMapLayer *& pLayer)
{
    for(auto one = m_userLayers.begin();one != m_userLayers.end();++one)
    {
        if(one->second == pLayer)
        {
            osgEarth::MapNode* pMapNode(nullptr);
            switch (m_emType)
            {
            case MAP_2D:
                pMapNode = m_pMap2DNode;
                break;
            case MAP_3D:
                pMapNode = m_pMap3DNode;
                break;
            }

            /// 从map中移除节点
            m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CMapModifyLayer(pMapNode,one->second->GetModelLayer(),false));
            delete one->second;

            /// 通知观察者
            for(auto oneObserver:m_listObserver)
            {
                oneObserver->RemoveLayer(one->first);
            }

            m_userLayers.erase(one);
            return(true);
        }
    }

    return(false);
}

void CMap::ClearLayers()
{
    osgEarth::MapNode* pMapNode(nullptr);
    switch (m_emType)
    {
    case MAP_2D:
        pMapNode = m_pMap2DNode;
        break;
    case MAP_3D:
        pMapNode = m_pMap3DNode;
        break;
    }

    for(auto one = m_userLayers.begin();one != m_userLayers.end();++one)
    {
        /// 从map中移除节点
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CMapModifyLayer(pMapNode,one->second->GetModelLayer(),false));
        delete one->second;
        m_userLayers.erase(one);

        /// 通知观察者
        for(auto oneObserver:m_listObserver)
        {
            oneObserver->RemoveLayer(one->first);
        }
    }
}

/// 更改地图类型
void CMap::ChangeMapType(MapType mapType)
{
    if(mapType == m_emType)
    {
        return;
    }
    else
    {
        m_emType = mapType;
        InitMap();

        for(auto one:m_listObserver)
        {
            one->MapTypeChanged(mapType);
        }
    }
}

/// 返回空间类的指针
ISpaceEnv *CMap::GetSpaceEnv()
{
    return(m_pSpaceEnv);
}

/// 设置地球自转
void CMap::SetEarthSelfRotate(bool bSelfRotate)
{
    if(m_bSelfRotate != bSelfRotate)
    {
        m_bSelfRotate = bSelfRotate;
    }
}

/// 初始化场景
void CMap::InitNode()
{
    ImplSceneGroup<IMap>::InitNode();

    m_pSpaceEnv = new CSpaceEnv(m_pSceneGraph);
    osg::Camera* pCamera = dynamic_cast<IOsgViewPoint*>(m_pSceneGraph->GetMainWindow()->GetMainViewPoint())
            ->GetOsgView()->getCamera();
    m_pSpaceEnv->SetMainCamara(pCamera);
    InitMap();
}

void CMap::InitMap()
{
    m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CClearChildNode(m_pGroup.get()));
    switch (m_emType)
    {
    case MAP_2D:
    {
        if(!m_p2DRoot.valid())
        {
            auto node = m_pSceneGraph->ResouceLoader()->LoadNode("Earth/Projected.earth");
            m_pMap2DNode = osgEarth::MapNode::findMapNode(node);
#if OSGEARTH_VERSION_GREATER_OR_EQUAL(3,0,0)
            m_pMap2DNode->open();
#endif
            m_p2DRoot = new osg::Group;
            m_p2DRoot->addChild(node);


            auto m_pLeftMatrixTransform = new osg::MatrixTransform;
            auto m_pRightMatrixTransform = new osg::MatrixTransform;
            m_pLeftMatrixTransform->setMatrix(osg::Matrix::translate(
                                                  osg::Vec3f(-m_pMap2DNode->getMap()->getProfile()->getExtent().width()
                                                             ,0.0f,0.0f)));


            m_pRightMatrixTransform->setMatrix(osg::Matrix::translate(
                                                   osg::Vec3f(m_pMap2DNode->getMap()->getProfile()->getExtent().width()
                                                              ,0.0f,0.0f)));

            m_pLeftMatrixTransform->addChild(node);
            m_pRightMatrixTransform->addChild(node);

            m_p2DRoot->addChild(m_pLeftMatrixTransform);
            m_p2DRoot->addChild(m_pRightMatrixTransform);
            osgEarth::GLUtils::setGlobalDefaults(m_p2DRoot->getOrCreateStateSet());
        }

        AddNode(m_pGroup.get(),m_p2DRoot);
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CMapNodeChanged(m_pMap3DNode,m_pMap2DNode,this));
    }
        break;
    case MAP_3D:
    {
        if(!m_pMap3DNode.valid())
        {
            auto node = m_pSceneGraph->ResouceLoader()->LoadNode("Earth/Geocentric.earth");
            m_pMap3DNode = osgEarth::MapNode::findMapNode(node);
#if OSGEARTH_VERSION_GREATER_OR_EQUAL(3,0,0)
            m_pMap3DNode->open();
#endif
            osgEarth::Util::LogarithmicDepthBuffer buffer;
            buffer.setUseFragDepth(true);
            buffer.install(m_pGroup.get());
            m_pSpaceEnv->Init();

            osgEarth::GLUtils::setGlobalDefaults(m_pMap3DNode->getOrCreateStateSet());
        }

        /// 增加更新
        AddNode(m_pGroup.get(),m_pMap3DNode.get());
        AddNode(m_pGroup.get(),m_pSpaceEnv->GetOsgNode());
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CMapNodeChanged(m_pMap2DNode,m_pMap3DNode,this));
    }
        break;
    }
}

static const char s_sMap2D[]="IMap2D";
static const char s_sMap3D[]="IMap3D";
ISceneNode* CreateNode(ISceneGraph*pSceneGraph,const std::string& sInterfaceName)
{
    if(sInterfaceName == s_sMap2D)
    {
        auto pMap = new CMap(pSceneGraph);
        pMap->SetType(MAP_2D);
        return(pMap);
    }
    else if(sInterfaceName == s_sMap3D)
    {
        auto pMap = new CMap(pSceneGraph);
        pMap->SetType(MAP_3D);
        return(pMap);
    }
    else
    {
        return(nullptr);
    }
}

bool QueryInterface(std::string& sInterfaceName)
{
    sInterfaceName = s_sMap2D;
    sInterfaceName += " ";
    sInterfaceName += s_sMap3D;
    return(true);
}
