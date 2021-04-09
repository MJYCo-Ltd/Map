#include <osgViewer/View>
#include <osg/MatrixTransform>
#include <osgEarth/Version>
#include <osgEarth/Terrain>
#include <osgEarth/Lighting>

#if OSGEARTH_VERSION_GREATER_OR_EQUAL(3,0,0)
#include <osgEarth/AutoClipPlaneHandler>
#else
#include <osgEarthUtil/AutoClipPlaneHandler>
#endif
#include <osgEarth/GLUtils>

#include <Satellite/Date.h>
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

bool MapEventCallback::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&,
                              osg::Object*, osg::NodeVisitor*)
{
    if(ea.MOVE == ea.getEventType())
    {
        static MapGeoPos pos;
        static float fX,fY;
        static short type(0);

        fX = ea.getX();
        if(ea.Y_INCREASING_UPWARDS == ea.getMouseYOrientation())
        {
            fY = ea.getYmax() - ea.getY();
        }
        else
        {
            fY = ea.getY();
        }
        m_pMap->ConvertCoord(fX,fY,pos,type);
        QMetaObject::invokeMethod(m_pMap,"MouseMove",Q_ARG(float,pos.fLon),Q_ARG(float,pos.fLat),Q_ARG(float,pos.fHeight));
    }
    return(false);
}

/// 析构函数
CMap::~CMap()
{
    auto pOsgViewPoint = dynamic_cast<IOsgViewPoint*>(m_pSceneGraph->GetMainWindow()->GetMainViewPoint());
    if(nullptr != pOsgViewPoint && nullptr != pOsgViewPoint->GetOsgView())
    {
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(
                    new CModifyViewHandler(pOsgViewPoint->GetOsgView(),m_pEventCallBack,false));
    }

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
bool CMap::ConvertCoord(float &fX, float &fY, MapGeoPos &geoPos, short TranType)
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
            if(m_pMap2DNode->getTerrain()->getWorldCoordsUnderMouse(pOsgViewPoint->GetOsgView(), fX, pViewPort ? pViewPort->height() - fY : fY, world))
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
            if(m_pMap3DNode->getTerrain()->getWorldCoordsUnderMouse(pOsgViewPoint->GetOsgView(), fX, pViewPort ? pViewPort->height() - fY : fY, world))
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
                fX = scrennPos.x();
                fY = pView->getCamera()->getViewport()->height() - scrennPos.y();
                return(true);
            }
            break;
        case MAP_3D:
            if(m_pMap3DNode->getMapSRS()->transformToWorld(osg::Vec3d(geoPos.fLon,geoPos.fLat,geoPos.fHeight),world))
            {
                osg::Matrixd _MVPW = pView->getCamera()->getViewMatrix() * pView->getCamera()->getProjectionMatrix()
                        * pView->getCamera()->getViewport()->computeWindowMatrix();

                osg::Vec3d scrennPos = world * _MVPW;
                fX = scrennPos.x();
                fY = pView->getCamera()->getViewport()->height() - scrennPos.y();
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
bool CMap::RemoveLayer(const std::string& sLayerName)
{
    auto findOne = m_userLayers.find(sLayerName);
    if(m_userLayers.end() == findOne)
    {
        return false;
    }
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
    m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CMapModifyLayer(pMapNode,findOne->second->GetModelLayer(),false));
    delete findOne->second;

    /// 通知观察者
    for(auto oneObserver:m_listObserver)
    {
        oneObserver->RemoveLayer(findOne->first);
    }

    m_userLayers.erase(findOne);
    return(true);
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
    auto pOsgViewPoint = dynamic_cast<IOsgViewPoint*>(m_pSceneGraph->GetMainWindow()->GetMainViewPoint());
    if(nullptr != pOsgViewPoint && nullptr != pOsgViewPoint->GetOsgView())
    {
        m_pEventCallBack = new MapEventCallback(this);
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(
                    new CModifyViewHandler(pOsgViewPoint->GetOsgView(),m_pEventCallBack,true));
    }
    InitMap();
}

/// 更新节点
void CMap::UpdateNode()
{
    if(m_bDateChanged)
    {
        m_pSpaceEnv->UpdateDate(m_dMJD);

        const Math::CVector& vSunPos = m_pSpaceEnv->GetSunPos();
        osg::Vec3 npos(vSunPos.GetX(),vSunPos.GetY(),vSunPos.GetZ());
        m_pLight->setPosition(osg::Vec4(npos,.0));
        m_pLightPosUniform->set(npos/npos.length());

        m_bDateChanged = false;
    }

    ImplSceneGroup<IMap>::UpdateNode();
}

/// 初始化地图
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
            m_p3DRoot = new osg::Group;
            auto node = m_pSceneGraph->ResouceLoader()->LoadNode("Earth/Geocentric.earth");
            m_pMap3DNode = osgEarth::MapNode::findMapNode(node);
#if OSGEARTH_VERSION_GREATER_OR_EQUAL(3,0,0)
            m_pMap3DNode->open();
#endif
            m_pSpaceEnv = new CSpaceEnv(m_pSceneGraph);
            osg::Camera* pCamera = dynamic_cast<IOsgViewPoint*>(m_pSceneGraph->GetMainWindow()->GetMainViewPoint())
                    ->GetOsgView()->getCamera();
            pCamera->addCullCallback(new osgEarth::Util::AutoClipPlaneCullCallback(m_pMap3DNode));
            m_pSpaceEnv->SetMainCamara(pCamera);
            m_pSpaceEnv->Init();

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

            osgEarth::GLUtils::setGlobalDefaults(m_pMap3DNode->getOrCreateStateSet());
        }

        /// 增加更新
        AddNode(m_pGroup.get(),m_p3DRoot.get());
        AddNode(m_pGroup.get(),m_pSpaceEnv->AsOsgSceneNode()->GetOsgNode());
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CMapNodeChanged(m_pMap2DNode,m_pMap3DNode,this));
    }
        break;
    }
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
    auto _ellipsoidModel = m_pMap3DNode->getMapSRS()->getEllipsoid();

    osgEarth::VirtualProgram* vp = osgEarth::VirtualProgram::getOrCreate(stateset);
    vp->setName( "SimpleSky Scene Lighting");
    m_pSceneGraph->ResouceLoader()->LoadVirtualProgram(vp,"GLSL/ONeilGround.glsl");

    float _innerRadius=osg::minimum(
                _ellipsoidModel->getRadiusPolar(),
                _ellipsoidModel->getRadiusEquator());
    float _outerRadius = _innerRadius * 1.025f;
    // calculate and apply the uniforms:
    // TODO: perhaps we can just hard-code most of these as GLSL consts.
    float r_wl = ::powf( .65f, 4.0f );
    float g_wl = ::powf( .57f, 4.0f );
    float b_wl = ::powf( .475f, 4.0f );
    osg::Vec3 RGB_wl(1.0f/r_wl, 1.0f/g_wl, 1.0f/b_wl);
    float Kr = 0.0025f;
    float Kr4PI = Kr * 4.0f * osg::PI;
    float Km = 0.0015f;
    float Km4PI = Km * 4.0f * osg::PI;
    float ESun = 15.0f;
    float MPhase = -.095f;
    float RayleighScaleDepth = 0.25f;
    int   Samples = 2;
    float Weather = 1.0f;

    float Scale = 1.0f / (_outerRadius - _innerRadius);

    //TODO: make all these constants. -gw
    stateset->getOrCreateUniform( "atmos_v3InvWavelength", osg::Uniform::FLOAT_VEC3 )->set( RGB_wl);
    stateset->getOrCreateUniform( "atmos_fInnerRadius",    osg::Uniform::FLOAT )->set(_innerRadius);
    stateset->getOrCreateUniform( "atmos_fInnerRadius2",   osg::Uniform::FLOAT )->set(_innerRadius * _innerRadius);
    stateset->getOrCreateUniform( "atmos_fOuterRadius",    osg::Uniform::FLOAT )->set(_outerRadius);
    stateset->getOrCreateUniform( "atmos_fOuterRadius2",   osg::Uniform::FLOAT )->set(_outerRadius * _outerRadius);
    stateset->getOrCreateUniform( "atmos_fKrESun",         osg::Uniform::FLOAT )->set(Kr * ESun);
    stateset->getOrCreateUniform( "atmos_fKmESun",         osg::Uniform::FLOAT )->set(Km * ESun);
    stateset->getOrCreateUniform( "atmos_fKr4PI",          osg::Uniform::FLOAT )->set(Kr4PI);
    stateset->getOrCreateUniform( "atmos_fKm4PI",          osg::Uniform::FLOAT )->set(Km4PI);
    stateset->getOrCreateUniform( "atmos_fScale",          osg::Uniform::FLOAT )->set(Scale);
    stateset->getOrCreateUniform( "atmos_fScaleDepth",     osg::Uniform::FLOAT )->set(RayleighScaleDepth);
    stateset->getOrCreateUniform( "atmos_fScaleOverScaleDepth", osg::Uniform::FLOAT )->set(Scale/RayleighScaleDepth);
    stateset->getOrCreateUniform( "atmos_g",               osg::Uniform::FLOAT )->set(MPhase);
    stateset->getOrCreateUniform( "atmos_g2",              osg::Uniform::FLOAT )->set(MPhase * MPhase);
    stateset->getOrCreateUniform( "atmos_nSamples",        osg::Uniform::INT )->set(Samples);
    stateset->getOrCreateUniform( "atmos_fSamples",        osg::Uniform::FLOAT )->set((float)Samples);
    stateset->getOrCreateUniform( "atmos_fWeather",        osg::Uniform::FLOAT )->set(Weather);

    // options:
    stateset->getOrCreateUniform("oe_sky_exposure",           osg::Uniform::FLOAT )->set(3.3f);
    stateset->getOrCreateUniform("oe_sky_ambientBoostFactor", osg::Uniform::FLOAT)->set(5.0f);
}

/// 鼠标移动消息
void CMap::MouseMove(float fLon, float fLat, float fHight)
{
    for(auto one:m_listObserver)
    {
        one->MousePos(fLon,fLat,fHight);
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
