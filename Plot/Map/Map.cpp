#include <osgViewer/View>
#include <osg/MatrixTransform>
#include <osgEarth/Version>
#include <osgEarth/Terrain>
#include <osgEarth/Lighting>
#include <osgEarth/GLUtils>
#include <osgEarth/LogarithmicDepthBuffer>

#include <GisMath/GisMath.h>
#include <Satellite/Date.h>
#include <Inner/IRender.h>
#include <Inner/ILoadResource.h>
#include <Inner/IOsgSceneNode.h>
#include <Inner/IOsgViewPoint.h>
#include <Inner/OsgExtern/OsgExtern.h>
#include <Inner/OsgExtern/IOsgMapSceneNode.h>
#include <Inner/IOsgViewPoint.h>
#include <SceneGraph/IWindow.h>
#include <SceneGraph/IViewPort.h>
#include <ISceneCore.h>
#include <Plot/Map/IMapObserver.h>

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

    if (m_pPreMapNode.valid())
    {
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new RemoveFromeScene(m_pPreMapNode));
        m_pPreMapNode = nullptr;
    }

    if (m_pCurMapNode.valid())
    {
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new RemoveFromeScene(m_pCurMapNode));
        m_pCurMapNode = nullptr;
    }

    /// 清空根节点
    m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new RemoveFromeScene(m_pGroup.get()));
}

/// 初始化地图路径
bool CMap::LoadUserMap(const std::string &sFileName, bool bRef)
{
    m_emType = MAP_USER;

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
        m_sUserMapPath = sRealFileName;
        LoadMap();
    }
    return(m_pCurMapNode.valid());
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
bool CMap::ConvertCoord(int &fX, int &fY, ScenePos &geoPos, short TranType)
{
    if(0 == TranType)
    {
        osg::Vec3d world;
        osgEarth::GeoPoint geoPoint;


        osg::Viewport* pViewPort = m_pView->getCamera()->getViewport();

        static float local_x(0.f), local_y(0.0f);
        const osg::Camera* camera = m_pView->getCameraContainingPosition(fX, pViewPort ? pViewPort->height() - fY : fY,
                                                                         local_x, local_y);
        if (!camera)
        {
            camera = m_pView->getCamera();
        }

        /// 构建矩阵
        osg::Matrixd matrix;

        /// 获取父节点
        osg::Matrix terrainRefFrame = osg::computeLocalToWorld(m_pCurMapNode->getParentalNodePaths()[0]);
        matrix.postMult(terrainRefFrame);

        matrix.postMult(camera->getViewMatrix());
        matrix.postMult(camera->getProjectionMatrix());

        double zNear = -1.0;
        double zFar = 1.0;
        if (camera->getViewport())
        {
            matrix.postMult(camera->getViewport()->computeWindowMatrix());
            zNear = 0.0, zFar = 1.0;
        }

        static osg::Matrixd inverse;
        inverse.invert(matrix);

        static osg::Vec3d vStartVertex,vDir, vUp,vEndVertex;
        camera->getViewMatrixAsLookAt(vStartVertex, vDir, vUp);
        vEndVertex = osg::Vec3d(local_x,local_y,zFar) * inverse;

        /// 重置picker类
        m_pPicker->reset();
        m_pPicker->setStart(vStartVertex);
        m_pPicker->setEnd(vEndVertex);

        /// 构建相交遍历器
        osgUtil::IntersectionVisitor iv(m_pPicker);
        iv.setTraversalMask(PICK_MASK);
        m_pCurMapNode->accept(iv);

        /// 判断是否相交
        if (m_pPicker->containsIntersections())
        {
            world = m_pPicker->getIntersections().begin()->getWorldIntersectPoint();
            geoPoint.fromWorld(m_pCurMapNode->getMapSRS(),world);
            geoPoint.makeGeographic();
            geoPos.fX = geoPoint.x();
            geoPos.fY = geoPoint.y();
            geoPos.fZ = geoPoint.z();
            return(true);
        }
        else
        {
            return(false);
        }
    }
    else if(1==TranType)
    {
        osg::Vec3d world;
        osgEarth::GeoPoint geoPoint(osgEarth::SpatialReference::create("wgs84"),geoPos.fX,geoPos.fY,geoPos.fZ),geoOut;

        if(geoPoint.transform(m_pCurMapNode->getMapSRS(),geoOut) &&
                m_pCurMapNode->getMapSRS()->transformToWorld(osg::Vec3d(geoOut.x(),geoOut.y(),geoOut.z()),world))
        {
            osg::Matrixd _MVPW = m_pView->getCamera()->getViewMatrix() * m_pView->getCamera()->getProjectionMatrix()
                    * m_pView->getCamera()->getViewport()->computeWindowMatrix();

            osg::Vec3d scrennPos = world * _MVPW;
            fX = scrennPos.x();
            fY = m_pView->getCamera()->getViewport()->height() - scrennPos.y();
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

    osgEarth::Terrain* pTerrain=m_pCurMapNode->getTerrain();

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
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CMapModifyLayer(m_pCurMapNode,pLayer->GetModelLayer(),true));

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
    m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CMapModifyLayer(m_pCurMapNode,itor->second->GetModelLayer(),false));
    delete itor->second;

    m_userLayers.erase(itor);
}

void CMap::ClearLayers()
{
    for(auto one = m_userLayers.begin();one != m_userLayers.end();++one)
    {
        /// 从map中移除节点
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CMapModifyLayer(m_pCurMapNode,one->second->GetModelLayer(),false));
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
    /// 如果是用户地图就返回
    if(MAP_USER == m_emType) return;

    if(mapType == m_emType)
    {
        return;
    }
    else
    {
        m_emType = mapType;
        LoadMap();

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

/// 设置黑夜颜色
void CMap::SetNightColor(const SceneColor &rColor)
{
    if(rColor != m_stNightColor)
    {
        m_stNightColor = rColor;
        if(m_pLight.valid())
        {
            m_pLight->setAmbient(osg::Vec4f(m_stNightColor.fR,m_stNightColor.fG
                                            ,m_stNightColor.fB,m_stNightColor.fA));
        }
    }
}

/// 鼠标移动消息
void CMap::MouseMove(MouseButtonMask, int nX, int nY)
{
    m_nX = nX;
    m_nY = nY;

    for(auto one:m_listObserver)
    {
        one->MousePos(m_stMousePos.fX,m_stMousePos.fY,m_stMousePos.fZ);
    }
}

/// 初始化场景
void CMap::InitNode()
{
    m_bOpenLight=true;
    m_pSceneGraph->GetMainWindow()->SubMessage(this);

    m_pView=m_pSceneGraph->GetMainWindow()->GetMainViewPoint()->AsOsgViewPoint()->GetOsgView();

    /// 设置拾取
    m_pPicker=new osgUtil::LineSegmentIntersector(osgUtil::Intersector::MODEL,osg::Vec3(),osg::Vec3());
    m_pPicker->setIntersectionLimit( osgUtil::Intersector::LIMIT_NEAREST );
    ImplSceneGroup<IMap>::InitNode();
    m_bCallOne=false;

    LoadMap();
}

/// 更新节点
void CMap::FrameCall()
{
    /// 地图更改
    if(m_bMapChanged)
    {
        if(m_pPreMapNode.valid())
        {
            for(auto one=m_userLayers.begin();one != m_userLayers.end();++one)
            {
                m_pPreMapNode->getMap()->removeLayer(one->second->GetModelLayer());
            }


            IOsgMapSceneNode::SetMapNode(m_pCurMapNode);
            m_pCurMapNode->getMap()->beginUpdate();
            for(auto one=m_userLayers.begin();one != m_userLayers.end();++one)
            {
                m_pCurMapNode->getMap()->addLayer(one->second->GetModelLayer());
                one->second->UpdateMapNode();
            }
            m_pCurMapNode->getMap()->endUpdate();
        }
        else
        {
            IOsgMapSceneNode::SetMapNode(m_pCurMapNode);
        }

        m_pGroup->removeChildren(0,m_pGroup->getNumChildren());
        /// 如果是三维地球
        if(m_pCurMapNode->isGeocentric())
        {
            if(!m_p3DRoot.valid())
            {
                m_p3DRoot = new osg::Group;
                Init3DLight();
                m_p3DRoot->addChild(m_pAtmosphere->GetNode());
            }
            m_p3DRoot->removeChild(m_pPreMapNode);
            m_p3DRoot->addChild(m_pCurMapNode);

            /// 添加到根节点
            m_pGroup->addChild(m_pSpaceEnv->AsOsgSceneNode()->GetOsgNode());
            m_pGroup->addChild(m_p3DRoot);

        }
        else /// 如果是二维地球
        {
            if(!m_pNoTran2DMapNode.valid())
            {
                m_pNoTran2DMapNode = new osg::Group;
            }
            m_pNoTran2DMapNode->removeChild(m_pPreMapNode);
            m_pNoTran2DMapNode->addChild(m_pCurMapNode);
            m_pGroup->addChild(m_pNoTran2DMapNode);

            auto dWidth = m_pCurMapNode->getMap()->getProfile()->getExtent().width();
            if(!m_pLeftTran.valid())
            {
                m_pLeftTran = new osg::MatrixTransform;
            }
            m_pLeftTran->removeChild(m_pPreMapNode);
            m_pLeftTran->addChild(m_pCurMapNode);
            m_pLeftTran->setMatrix(osg::Matrix::translate(osg::Vec3d(-dWidth,0.,0.)));

            /// 如果右边的没有
            if(!m_pRightTran.valid())
            {
                m_pRightTran = new osg::MatrixTransform;
            }
            m_pRightTran->removeChild(m_pPreMapNode);
            m_pRightTran->addChild(m_pCurMapNode);
            m_pRightTran->setMatrix(osg::Matrix::translate(osg::Vec3d(dWidth,0.,0.)));


            /// 添加到根节点
            m_pGroup->addChild(m_pRightTran);
            m_pGroup->addChild(m_pLeftTran);
        }

        ///切换视点
        IOsgViewPoint* pViewPoint = dynamic_cast<IOsgViewPoint*>(m_pSceneGraph->GetMainWindow()->GetMainViewPoint());
        if(nullptr != pViewPoint)
        {
            pViewPoint->ViewPointTypeChanged(m_pCurMapNode->isGeocentric() ? IOsgViewPoint::View_3DMap : IOsgViewPoint::View_2DMap);
        }

        m_bMapChanged=false;
    }

    /// 时间更改
    if(m_bDateChanged)
    {
        m_pSpaceEnv->UpdateDate(m_dMJD);

        const Math::CVector& vSunPos = m_pSpaceEnv->GetSunPos();
        osg::Vec3 npos(vSunPos.GetX(),vSunPos.GetY(),vSunPos.GetZ());
        m_pLight->setPosition(osg::Vec4(npos,.0));
        m_pLightPosUniform->set(npos/npos.length());

        m_bDateChanged = false;
    }

    /// 每一帧更新一次
    if(m_pCurMapNode.valid())
    {
        if(m_pCurMapNode->isGeocentric())
        {
            static osgEarth::LogarithmicDepthBuffer s_logDepthBuffer;

            osg::Vec3d eye,center,up;
            m_pView->getCamera()->getViewMatrixAsLookAt(eye,center,up);
            if(eye.length() - R_Earth < 3000)
            {
                if(!m_bInstelld)
                {
                    s_logDepthBuffer.install(m_pCurMapNode);
                    m_bInstelld=true;
                }
            }
            else
            {
                if(m_bInstelld)
                {
                    s_logDepthBuffer.uninstall(m_pCurMapNode);
                    m_bInstelld=false;
                }
            }
        }

        ConvertCoord(m_nX,m_nY,m_stMousePos,0);
    }


    ImplSceneGroup<IMap>::FrameCall();
}

/// 加载地图数据
void CMap::LoadMap()
{
    std::string sFileName;
    switch(m_emType)
    {
    case MAP_2D:
        sFileName = GetDataPath()+ "Earth/Projected.earth";
        break;
    case MAP_3D:
        sFileName = GetDataPath()+ "Earth/Geocentric.earth";
        break;
    case MAP_USER:
        sFileName = m_sUserMapPath;
        break;
    }

    /// 保存之前的状态
    m_pPreMapNode = m_pCurMapNode;

    auto pNode = m_pSceneGraph->ResouceLoader()->LoadNode(sFileName,false);

    /// 如果节点不等于0
    if(nullptr != pNode)
    {
        m_pCurMapNode = osgEarth::MapNode::findMapNode(pNode);
        if(nullptr != m_pCurMapNode)
        {
#if OSGEARTH_VERSION_GREATER_OR_EQUAL(3,0,0)
            m_pCurMapNode->open();
#endif
            osgEarth::GLUtils::setGlobalDefaults(m_pCurMapNode->getOrCreateStateSet());
            m_bMapChanged=true;
            if(m_pCurMapNode->isGeocentric())
            {
                if(nullptr == m_pSpaceEnv)
                {
                    m_pSpaceEnv = new CSpaceEnv(m_pSceneGraph);
                    osg::Camera* pCamera = m_pView->getCamera();
                    m_pSpaceEnv->SetMainCamara(pCamera);
                    m_pSpaceEnv->Init();

                    time_t timep;

                    /// 更新时间
                    time(&timep);
                    auto p = gmtime(&timep);
                    Aerospace::CDate data(p->tm_year+1900,p->tm_mon+1
                                          ,p->tm_mday,p->tm_hour
                                          ,p->tm_min,p->tm_sec,UTC);

                    m_dMJD = data.GetMJD();
                    DateChanged();
                }

                if(nullptr == m_pAtmosphere)
                {
                    m_pAtmosphere = new CAtmosphere(m_pSceneGraph);
                    m_pAtmosphere->MakeAtmosphere();
                }
            }
        }
    }
}

/// 初始化光线
void CMap::Init3DLight()
{
    osg::Vec3f lightPos(0.0f, 0.0f, 1.0f);
    osg::LightSource* lightSource = new osg::LightSource();
    m_pLight = new osgEarth::LightGL3( 0 );
    m_pLight->setPosition( osg::Vec4f(0.0f, 0.0f, 1.0f, 0.0f) );
    m_pLight->setAmbient ( osg::Vec4f(m_stNightColor.fR,m_stNightColor.fG
                                      ,m_stNightColor.fB,m_stNightColor.fA) );
    m_pLight->setDiffuse ( osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f) );
    m_pLight->setSpecular( osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f) );
    lightSource->setLight(m_pLight);

    lightSource->setCullingActive(false);
    m_p3DRoot->addChild(lightSource);

    lightSource->addCullCallback(new osgEarth::LightSourceGL3UniformGenerator);
    osg::StateSet* stateset = m_pGroup->getOrCreateStateSet();
    m_pLightPosUniform = stateset->getOrCreateUniform("atmos_v3LightDir",osg::Uniform::FLOAT_VEC3);
    m_pLightPosUniform->set( lightPos / lightPos.length());

    float _outerRadius = osg::minimum(R_Earth2,R_Earth) * 1.025f;
    stateset->getOrCreateUniform( "atmos_fOuterRadius",    osg::Uniform::FLOAT )->set(_outerRadius);
    stateset->getOrCreateUniform( "atmos_fOuterRadius2",   osg::Uniform::FLOAT )->set(_outerRadius * _outerRadius);

    stateset = m_p3DRoot->getOrCreateStateSet();
    stateset->setDefine("OE_NUM_LIGHTS", "1");

    osgEarth::VirtualProgram* vp = osgEarth::VirtualProgram::getOrCreate(stateset);
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
