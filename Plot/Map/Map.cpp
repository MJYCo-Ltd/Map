#include <osgViewer/View>
#include <osg/MatrixTransform>
#include <osgEarth/Version>
#include <osgEarth/Terrain>
#include <osgEarth/Lighting>
#include <osgEarth/GLUtils>

#include <GisMath/GisMath.h>
#include <Satellite/Date.h>
#include <Inner/IRender.h>
#include <Inner/ILoadResource.h>
#include <Inner/IOsgSceneNode.h>
#include <Inner/IOsgViewPoint.h>
#include <Inner/OsgExtern/OsgExtern.h>
#include <Inner/OsgExtern/IOsgMapSceneNode.h>
#include <Inner/OsgExtern/MapNodeCullBack.h>
#include <Inner/IOsgViewPoint.h>
#include <SceneGraph/IWindow.h>
#include <SceneGraph/IViewPort.h>
#include <ISceneCore.h>
#include <Plot/Map/IMapObserver.h>

#include "MapNodeChanged.h"
#include "MapModifyLayer.h"
#include "SpaceEnv.h"
#include "Atmosphere.h"
#include "Map.h"
#include "MapLayer.h"
#include "MapModelLayer.h"

/// 析构函数
CMap::~CMap()
{
    m_pSceneGraph->GetMainWindow()->UnSubMessage(this);

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

/// 初始化地图路径
bool CMap::LoadUserMap(const std::string &sFileName, bool bRef)
{
    std::string sRealFileName;
    if(bRef)
    {
        sRealFileName =  GetDataPath() + sFileName;
    }
    else
    {
        sRealFileName = sFileName;
    }

    /// 如果两者不等
    if(sRealFileName != m_sUserMapPath)
    {
    }
    return(true);
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
bool CMap::ConvertCoord(float &fX, float &fY, ScenePos &geoPos, short TranType)
{
    if(0 == TranType)
    {
        osg::Vec3d world;
        osgEarth::GeoPoint geoPoint;
        auto pOsgViewPoint = m_pSceneGraph->GetMainWindow()->GetMainViewPoint()->AsOsgViewPoint();
        if(!pOsgViewPoint || !pOsgViewPoint->GetOsgView())
        {
            return(false);
        }

        osg::Viewport* pViewPort = pOsgViewPoint->GetOsgView()->getCamera()->getViewport();
        if(nullptr != GetMapNode()->getTerrain() &&
                GetMapNode()->getTerrain()->getWorldCoordsUnderMouse(pOsgViewPoint->GetOsgView(),
                                                                fX, pViewPort ? pViewPort->height() - fY : fY, world))
        {
            geoPoint.fromWorld(GetMapNode()->getMapSRS(),world);
            geoPoint.makeGeographic();
            geoPos.fX = geoPoint.x();
            geoPos.fY = geoPoint.y();
            geoPos.fZ = geoPoint.z();
            return (true);
        }
        else
        {
            return(false);
        }
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
        osgEarth::GeoPoint geoPoint(osgEarth::SpatialReference::create("wgs84"),geoPos.fX,geoPos.fY,geoPos.fZ),geoOut;

        if(geoPoint.transform(GetMapNode()->getMapSRS(),geoOut) &&
                GetMapNode()->getMapSRS()->transformToWorld(osg::Vec3d(geoOut.x(),geoOut.y(),geoOut.z()),world))
        {
            osg::Matrixd _MVPW = pView->getCamera()->getViewMatrix() * pView->getCamera()->getProjectionMatrix()
                    * pView->getCamera()->getViewport()->computeWindowMatrix();

            osg::Vec3d scrennPos = world * _MVPW;
            fX = scrennPos.x();
            fY = pView->getCamera()->getViewport()->height() - scrennPos.y();
            return(true);
        }
        else
        {
            return(false);
        }
    }
    else
    {
        return(false);
    }
}

float CMap::GetHeight(float fLon, float fLat)
{
    static osgEarth::SpatialReference* s_pWgs84 = osgEarth::SpatialReference::get("wgs84");

    osgEarth::Terrain* pTerrain=GetMapNode()->getTerrain();

    if(nullptr == pTerrain)
    {
        return(0);
    }

    static double dHeight;
    pTerrain->getHeight(s_pWgs84,fLon,fLat,&dHeight);
    return(dHeight);
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
        CMapLayer* pLayer = new CMapLayer(sLayerName,m_pSceneGraph);
        m_userLayers[sLayerName] = pLayer;

        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CMapModifyLayer(GetMapNode(),pLayer->GetModelLayer(),true));

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
            RemoveLayer(one);
            pLayer=nullptr;
            return(true);
        }
    }

    return(false);
}

bool CMap::RemoveLayer(const std::string& sLayerName)
{
    auto findOne = m_userLayers.find(sLayerName);
    if(m_userLayers.end() == findOne)
    {
        return false;
    }

    RemoveLayer(findOne);

    return(true);
}

void CMap::RemoveLayer(UserLayers::iterator itor)
{
    /// 通知观察者
    for(auto oneObserver:m_listObserver)
    {
        oneObserver->RemoveLayer(itor->first);
    }

    /// 从map中移除节点
    m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CMapModifyLayer(GetMapNode(),itor->second->GetModelLayer(),false));
    delete itor->second;

    m_userLayers.erase(itor);
}

/// 获取正确的mapNode
osgEarth::MapNode *CMap::GetMapNode()
{
    switch (m_emType)
    {
    case MAP_2D:
        return(m_pMap2DNode);
    case MAP_3D:
        return(m_pMap3DNode);
    case MAP_USER:
        return(m_pMapUser);
        break;
    }
}

void CMap::ClearLayers()
{
    osgEarth::MapNode* pMapNode=GetMapNode();

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
    if(MAP_USER == m_emType) return;

    if(mapType == m_emType)
    {
        return;
    }
    else
    {
        m_emType = mapType;
        LoadMap();

        IOsgViewPoint* pViewPoint = dynamic_cast<IOsgViewPoint*>(m_pSceneGraph->GetMainWindow()->GetMainViewPoint());
        if(nullptr != pViewPoint)
        {
            pViewPoint->ViewPointTypeChanged(MAP_3D == m_emType ? IOsgViewPoint::View_3DMap : IOsgViewPoint::View_2DMap);
        }

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

void CMap::SetShowAtmosphere(bool bVisible)
{
    m_pAtmosphere->SetVisible(bVisible);
}

/// 鼠标移动消息
void CMap::MouseMove(MouseButtonMask, int nX, int nY)
{
    m_pUpdateCallBack->SetMousePos(nX,nY);
    osgEarth::MapNode* pMapNode=GetMapNode();

    if(pMapNode&&pMapNode->getUserData())
    {
        YtyUserData* pUserData = static_cast<YtyUserData*>(pMapNode->getUserData());
        for(auto one:m_listObserver)
        {
            one->MousePos(pUserData->GetDx(),pUserData->GetDy(),pUserData->GetDz());
        }
    }
}

/// 初始化场景
void CMap::InitNode()
{
    m_bOpenLight=true;
    m_pUpdateCallBack = new CMapNodeCullBack(m_pSceneGraph);
    m_pSceneGraph->GetMainWindow()->SubMessage(this);
    ImplSceneGroup<IMap>::InitNode();
    LoadMap();
}

/// 更新节点
void CMap::FrameCall()
{
    if(m_bDateChanged)
    {
        m_pSpaceEnv->UpdateDate(m_dMJD);
        m_pSceneGraph->GetMainWindow()->GetMainViewPoint()->GetViewPoint();

        const Math::CVector& vSunPos = m_pSpaceEnv->GetSunPos();
        osg::Vec3 npos(vSunPos.GetX(),vSunPos.GetY(),vSunPos.GetZ());
        m_pLight->setPosition(osg::Vec4(npos,.0));
        m_pLightPosUniform->set(npos/npos.length());

        m_bDateChanged = false;
    }
    ImplSceneGroup<IMap>::FrameCall();
}

/// 初始化地图
void CMap::InitMap(osgEarth::MapNode* pMapNode)
{
    m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CClearChildNode(m_pGroup.get()));

    bool bIs3D = pMapNode->isGeocentric();
    if(MAP_USER == m_emType)
    {
        if(bIs3D)
        {
        }
        else
        {
        }
    }
    else
    {
        switch (m_emType)
        {
        case MAP_2D:
        {
            if(!m_p2DRoot.valid())
            {
                m_p2DRoot = new osg::Group;
                m_p2DRoot->addChild(m_pMap2DNode);


                auto m_pLeftMatrixTransform = new osg::MatrixTransform;
                auto m_pRightMatrixTransform = new osg::MatrixTransform;
                m_pLeftMatrixTransform->setMatrix(osg::Matrix::translate(
                                                      osg::Vec3f(-m_pMap2DNode->getMap()->getProfile()->getExtent().width()
                                                                 ,0.0f,0.0f)));


                m_pRightMatrixTransform->setMatrix(osg::Matrix::translate(
                                                       osg::Vec3f(m_pMap2DNode->getMap()->getProfile()->getExtent().width()
                                                                  ,0.0f,0.0f)));

                m_pLeftMatrixTransform->addChild(m_pMap2DNode);
                m_pRightMatrixTransform->addChild(m_pMap2DNode);

                m_p2DRoot->addChild(m_pLeftMatrixTransform);
                m_p2DRoot->addChild(m_pRightMatrixTransform);
            }

            AddNode(m_pGroup.get(),m_p2DRoot);
            if(m_pMap3DNode.valid())
            {
                m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CMapNodeChanged(m_pMap3DNode,m_pMap2DNode,this));
            }
            else
            {
                IOsgMapSceneNode::SetMapNode(m_pMap2DNode);
            }
        }
            break;
        case MAP_3D:
        {
            if(!m_p3DRoot.valid())
            {
                m_p3DRoot = new osg::Group;

                m_pSpaceEnv = new CSpaceEnv(m_pSceneGraph);
                osg::Camera* pCamera = dynamic_cast<IOsgViewPoint*>(m_pSceneGraph->GetMainWindow()->GetMainViewPoint())
                        ->GetOsgView()->getCamera();
                m_pSpaceEnv->SetMainCamara(pCamera);
                m_pSpaceEnv->Init();

                m_pAtmosphere = new CAtmosphere(m_pSceneGraph);
                m_pAtmosphere->MakeAtmosphere();
                m_p3DRoot->addChild(m_pAtmosphere->GetNode());


                Init3DLight();

                time_t timep;

                /// 更新时间
                time(&timep);
                auto p = gmtime(&timep);
                Aerospace::CDate data(p->tm_year+1900,p->tm_mon+1
                                      ,p->tm_mday,p->tm_hour
                                      ,p->tm_min,p->tm_sec,UTC);

                m_dMJD = data.GetMJD();
                DateChanged();
                m_p3DRoot->addChild(m_pMap3DNode);
            }

            /// 增加更新
            AddNode(m_pGroup.get(),m_p3DRoot.get());
            AddNode(m_pGroup.get(),m_pSpaceEnv->AsOsgSceneNode()->GetOsgNode());
            if(m_pMap2DNode.valid())
            {
                m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CMapNodeChanged(m_pMap2DNode,m_pMap3DNode,this));
            }
            else
            {
                IOsgMapSceneNode::SetMapNode(m_pMap3DNode);
            }
        }
            break;
        }
    }
}

/// 加载地图数据
void CMap::LoadMap()
{
    osgEarth::MapNode* pEarthMap{};

    switch (m_emType)
    {
    case MAP_2D:
        pEarthMap = m_pMap2DNode.valid() ? m_pMap2DNode.get() : LoadEarthFile();
        break;
    case MAP_3D:
        pEarthMap = m_pMap3DNode.valid() ? m_pMap3DNode.get() : LoadEarthFile();
        break;
    case MAP_USER:
        pEarthMap = m_pMapUser.valid() ? m_pMapUser.get() : LoadEarthFile();
        break;
    }

    if(nullptr != pEarthMap)
    {
        InitMap(pEarthMap);
    }
}

/// 加载地图数据
osgEarth::MapNode *CMap::LoadEarthFile()
{
    osg::Node* pNode{};
    switch(m_emType)
    {
    case MAP_2D:
        pNode = m_pSceneGraph->ResouceLoader()->LoadNode("Earth/Projected.earth");
        break;
    case MAP_3D:
        pNode = m_pSceneGraph->ResouceLoader()->LoadNode("Earth/Geocentric.earth");
        break;
    case MAP_USER:
        pNode = m_pSceneGraph->ResouceLoader()->LoadNode(m_sUserMapPath,false);
        break;
    }

    /// 如果节点不等于0
    if(nullptr != pNode)
    {
        auto pEarthMap = osgEarth::MapNode::findMapNode(pNode);
        if(nullptr != pEarthMap)
        {
#if OSGEARTH_VERSION_GREATER_OR_EQUAL(3,0,0)
            pEarthMap->open();
#endif
            switch(m_emType)
            {
            case MAP_2D:
                m_pMap2DNode = pEarthMap;
                break;
            case MAP_3D:
                m_pMap3DNode = pEarthMap;
                break;
            case MAP_USER:
                m_pMapUser = pEarthMap;
                break;
            }
            pEarthMap->addEventCallback(m_pUpdateCallBack);
            osgEarth::GLUtils::setGlobalDefaults(pEarthMap->getOrCreateStateSet());
        }
        return(pEarthMap);
    }

    return(nullptr);
}

/// 初始化光线
void CMap::Init3DLight()
{
    osg::Vec3f lightPos(0.0f, 0.0f, 1.0f);
    osg::LightSource* lightSource = new osg::LightSource();
    m_pLight = new osgEarth::LightGL3( 0 );
    m_pLight->setPosition( osg::Vec4f(0.0f, 0.0f, 1.0f, 0.0f) );
    m_pLight->setAmbient ( osg::Vec4f(0.1f, 0.1f, 0.1f, 1.0f) );
    m_pLight->setDiffuse ( osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f) );
    m_pLight->setSpecular( osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f) );
    lightSource->setLight(m_pLight);

    lightSource->setCullingActive(false);
    m_p3DRoot->addChild(lightSource);

    lightSource->addCullCallback(new osgEarth::LightSourceGL3UniformGenerator);
    osg::StateSet* stateset = m_p3DRoot->getOrCreateStateSet();
    m_pLightPosUniform = stateset->getOrCreateUniform("atmos_v3LightDir",osg::Uniform::FLOAT_VEC3);
    m_pLightPosUniform->set( lightPos / lightPos.length());

    stateset->setDefine("OE_NUM_LIGHTS", "1");

    osgEarth::VirtualProgram* vp = osgEarth::VirtualProgram::getOrCreate(stateset);
    vp->setName( "SimpleSky Scene Lighting");
    m_pSceneGraph->ResouceLoader()->LoadVirtualProgram(vp,"GLSL/PhongLighting.glsl");

}

static const char s_sMap2D[]="IMap2D";
static const char s_sMap3D[]="IMap3D";
static const char s_sMapUser[]="IMapUser";
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
    else if(sInterfaceName == s_sMapUser)
    {
        auto pMap = new CMap(pSceneGraph);
        pMap->SetType(MAP_USER);
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
    sInterfaceName += " ";
    sInterfaceName += s_sMapUser;
    return(true);
}
