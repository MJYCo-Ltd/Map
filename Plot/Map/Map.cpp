#include <osgViewer/View>
#include <osg/MatrixTransform>
#include <osgEarth/Version>
#include <osgEarth/Terrain>
#include <osgEarth/Lighting>
#include <osgEarth/GLUtils>
#include <osgEarth/LogarithmicDepthBuffer>
#include <osgEarth/EarthManipulator>

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

    CMap::ClearLayers();

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
    m_setObserver.Add(pMsgObr);
}

/// 取消订阅
void CMap::UnSubMessage(IMapMessageObserver *pMsgObr)
{
    m_setObserver.Remove(pMsgObr);
}

/// 坐标转换
bool CMap::ConvertCoord(int &fX, int &fY, ScenePos &geoPos, short TranType)
{

    osg::Viewport* pViewPort = m_pView->getCamera()->getViewport();

    static float local_x(0.f), local_y(0.0f);
    const osg::Camera* camera = m_pView->getCameraContainingPosition(fX, pViewPort ? pViewPort->height() - fY : fY,
                                                                     local_x, local_y);
    if (!camera)
    {
        camera = m_pView->getCamera();
    }

    if(0 == TranType)
    {
        if(m_bIs3D)
        {
            static osg::Vec3d vStartVertex,vIgnore;
            /// 构建矩阵
            static osg::Matrixd matrix,terrainRefFrame,inverse;

            matrix.makeIdentity();

            /// 获取父节点
            terrainRefFrame = osg::computeLocalToWorld(m_pCurMapNode->getParentalNodePaths()[0]);
            matrix.postMult(terrainRefFrame);

            matrix.postMult(camera->getViewMatrix());
            matrix.postMult(camera->getProjectionMatrix());

            matrix.postMult(pViewPort->computeWindowMatrix());
            inverse.invert(matrix);

            static osg::Vec3d vCenter,vFar(1.,1.,1.);
            camera->getViewMatrixAsLookAt(vStartVertex, vIgnore, vIgnore);
            vFar.x() = local_x;
            vFar.y() = local_y;

            /// 重置picker类
            m_pPicker->reset();
            m_pPicker->setStart(vStartVertex);
            m_pPicker->setEnd(vFar * inverse);

            /// 构建相交遍历器
            osgUtil::IntersectionVisitor iv(m_pPicker);
            iv.setTraversalMask(PICK_MASK);
            m_pCurMapNode->accept(iv);

            /// 判断是否相交
            if (m_pPicker->containsIntersections())
            {
                static osgEarth::GeoPoint geoPoint;
                vIgnore = m_pPicker->getIntersections().begin()->getWorldIntersectPoint();
                geoPoint.fromWorld(m_pCurMapNode->getMapSRS(),vIgnore);
                geoPos.dX = geoPoint.x();
                geoPos.dY = geoPoint.y();
                geoPos.dZ = geoPoint.z();
                return(true);
            }

            return(false);
        }
        else
        {
            static double dHafX,dHafY,dIgnore;
            camera->getProjectionMatrixAsOrtho(dIgnore,dHafX,dIgnore,dHafY,dIgnore,dIgnore);
            auto pEarthManipulator = static_cast<osgEarth::Util::EarthManipulator*>(m_pView->getCameraManipulator());

            static osgEarth::GeoPoint geoPoint;
            geoPoint = pEarthManipulator->getViewpoint().focalPoint().get();
            geoPoint.x() += (local_x*2/pViewPort->width()-1)*dHafX;
            geoPoint.y() += (local_y*2/pViewPort->height()-1)*dHafY;

            if(geoPoint.x() > m_extent.xMax())
            {
                geoPoint.x() -= m_extent.width();
            }

            if(geoPoint.x() < m_extent.xMin())
            {
                geoPoint.x() += m_extent.width();
            }
            geoPoint.makeGeographic();


            geoPos.dX = geoPoint.x();
            geoPos.dY = geoPoint.y();
            geoPos.dZ = 0;

            return(true);
        }
    }
    else if(1==TranType)
    {
        if(m_bIs3D)
        {
            osg::Vec3d world;
            osgEarth::GeoPoint geoPoint(IOsgMapSceneNode::s_pWGS84,geoPos.dX,geoPos.dY,geoPos.dZ),geoOut;

            if(geoPoint.transform(m_pCurMapNode->getMapSRS(),geoOut) &&
                    m_pCurMapNode->getMapSRS()->transformToWorld(osg::Vec3d(geoOut.x(),geoOut.y(),geoOut.z()),world))
            {
                osg::Matrixd _MVPW = camera->getViewMatrix() * camera->getProjectionMatrix()
                        * camera->getViewport()->computeWindowMatrix();

                osg::Vec3d scrennPos = world * _MVPW;
                fX = scrennPos.x();
                fY = pViewPort->height() - scrennPos.y();
                return(true);
            }
            else
            {
                return(false);
            }
        }
    }

    return(false);
}

float CMap::GetHeight(float fLon, float fLat)
{
    osgEarth::Terrain* pTerrain=m_pCurMapNode->getTerrain();

    if(nullptr == pTerrain)
    {
        return(0);
    }

    static double dHeight;
    pTerrain->getHeight(IOsgMapSceneNode::s_pWGS84,fLon,fLat,&dHeight);
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
        pLayer->SetRenderOrder(m_pCurMapNode->getMap()->getNumLayers());
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CMapModifyLayer(m_pCurMapNode,pLayer->GetModelLayer(),true));

        /// 通知观察者
        static std::vector<IMapMessageObserver*> allObserver;
        m_setObserver.GetAll(allObserver);
        for(auto one:allObserver)
        {
            one->AddLayer(sLayerName);
        }
        allObserver.clear();

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
    static std::vector<IMapMessageObserver*> allObserver;
    m_setObserver.GetAll(allObserver);
    for(auto one:allObserver)
    {
        one->RemoveLayer(itor->first);
    }
    allObserver.clear();

    /// 从map中移除节点
    m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CMapModifyLayer(m_pCurMapNode,itor->second->GetModelLayer(),false));
    delete itor->second;

    m_userLayers.erase(itor);
}
void CMap::GetXYZ(double dLon,double dLat,double dHeight,double& x,double& y ,double&z)
{
    if(m_emType == MAP_3D)
    {
        osg::Vec3d vIn(dLon,dLat,dHeight);
        osg::Vec3d vOut;
        m_pCurMapNode->getMapSRS()->transformToWorld(vIn,vOut);
        x = vOut.x();
        y = vOut.y();
        z = vOut.z();
    }
    else
    {
        std::vector<osg::Vec3d> input;
        osg::Vec3d vIn(dLon,dLat,dHeight);
        input.push_back(vIn);
        IOsgMapSceneNode::s_pWGS84.get()->transform(input,m_pCurMapNode->getMapSRS());
        vIn = input[0];
        x = vIn.x();
        y = vIn.y();
        z = vIn.z();
    }
}
void CMap::GetPOS(double x,double y,double z,double& dLon,double& dLat ,double&dHeight  )
{
    if(m_emType == MAP_3D)
    {
        osg::Vec3d vIn(x,y,z);
        osg::Vec3d vOut;
        m_pCurMapNode->getMapSRS()->transformFromWorld(vIn,vOut);
        dLon = vOut.x();
        dLat = vOut.y();
        dHeight = vOut.z();
    }
    else
    {
        std::vector<osg::Vec3d> input;
        osg::Vec3d vIn(x,y,z);
        input.push_back(vIn);

        m_pCurMapNode->getMapSRS()->transform(input,IOsgMapSceneNode::s_pWGS84.get());
        vIn = input[0];
        dLon = vIn.x();
        dLat = vIn.y();
        dHeight = vIn.z();
    }
}
void CMap::getScreenXY(int& x,int& y)
{
    osg::Vec3d out_world;

    m_pCurMapNode->getTerrain()->getWorldCoordsUnderMouse(m_pView.get(), x, y, out_world);

    osgEarth::GeoPoint eqcPos;

    eqcPos.fromWorld(m_pCurMapNode->getMapSRS(), out_world);
    eqcPos.makeGeographic();
   
    x = eqcPos.vec3d().x();
    y = eqcPos.vec3d().y();
}
void CMap::ClearLayers()
{
    static std::vector<IMapMessageObserver*> allObserver;
    m_setObserver.GetAll(allObserver);

    for(auto one = m_userLayers.begin();one != m_userLayers.end();)
    {
        for(auto oneObserver:allObserver)
        {
            oneObserver->RemoveLayer(one->first);
        }
        /// 从map中移除节点
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CMapModifyLayer(m_pCurMapNode,one->second->GetModelLayer(),false));
        delete one->second;

       one =  m_userLayers.erase(one);

    }
    allObserver.clear();
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

        static std::vector<IMapMessageObserver*> allObserver;
        m_setObserver.GetAll(allObserver);
        for(auto one:allObserver)
        {
            one->MapTypeChanged(mapType);
        }
        allObserver.clear();
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

void CMap::DateChanged()
{
    m_pSpaceEnv->UpdateDate(m_dMJD);
}

/// 鼠标移动消息
void CMap::MouseMove(MouseButtonMask, int nX, int nY)
{
    m_nX = nX;
    m_nY = nY;

    m_bMouseMove=true;
    static std::vector<IMapMessageObserver*> allObserver;
    m_setObserver.GetAll(allObserver);
    for(auto one:allObserver)
    {
        one->MousePos(m_stMousePos.dX,m_stMousePos.dY,m_stMousePos.dZ);
    }
    allObserver.clear();
}
void CMap::MouseDown(MouseButtonMask type, int nX, int nY)
{
    m_nX = nX;
    m_nY = nY;
    static std::vector<IMapMessageObserver*> allObserver;
    m_setObserver.GetAll(allObserver);
    for(auto one:allObserver)
    {
        one->MouseDown(type,m_stMousePos.dX,m_stMousePos.dY,m_stMousePos.dZ);
    }
    allObserver.clear();
}

void CMap::MouseDblClick(MouseButtonMask  type, int nX, int nY)
{
    m_nX = nX;
    m_nY = nY;
    static std::vector<IMapMessageObserver*> allObserver;
    m_setObserver.GetAll(allObserver);

    auto iter = allObserver.begin();
    while (iter != allObserver.end())
    {
        auto temIter = iter;
        temIter++;
        (*iter)->MouseDblClick(type,m_stMousePos.dX,m_stMousePos.dY,m_stMousePos.dZ);
        iter=temIter;
    }

    allObserver.clear();
}

void CMap::SetLockView(bool bLock)
{
     dynamic_cast<IOsgViewPoint*>(m_pSceneGraph->GetMainWindow()->GetMainViewPoint())->SetLockView(bLock);
}

void CMap::SetViewPos(const ScenePos & pos)
{

}

double CMap::getEvelation(double dLon, double dLat)
{
    double dHeight;
    m_pCurMapNode->getTerrain()->getHeight(m_pCurMapNode->getMapSRS(),dLon,dLat,&dHeight);
    return dHeight;
}
/// 初始化场景
void CMap::InitNode()
{
    ImplSceneGroup<IMap>::InitNode();
    m_pDepth = new osg::Depth;
    m_pGroup->getOrCreateStateSet()->setAttribute(m_pDepth);
    m_bOpenLight=true;

    m_pView=m_pSceneGraph->GetMainWindow()->GetMainViewPoint()->AsOsgViewPoint()->GetOsgView();

    /// 设置拾取
    m_pPicker=new osgUtil::LineSegmentIntersector(osgUtil::Intersector::MODEL,osg::Vec3(),osg::Vec3());
    m_pPicker->setIntersectionLimit( osgUtil::Intersector::LIMIT_NEAREST );
    m_bCallOne=false;
    m_stNightColor.fR = m_stNightColor.fG = m_stNightColor.fB = 0.1f;
    LoadMap();
    m_pSceneGraph->GetMainWindow()->SubMessage(this);
    m_pSceneGraph->SceneGraphRender()->SubMessage(this);
}

/// 更新节点
void CMap::FrameCall()
{
    /// 地图更改
    if(m_bMapChanged)
    {
        IOsgMapSceneNode::SetMapNode(m_pCurMapNode,m_pSceneGraph);

        if(m_pPreMapNode.valid())
        {
            for(auto one=m_userLayers.begin();one != m_userLayers.end();++one)
            {
                one->second->GetModelLayer()->SetCanDelete(false);
            }

            for(auto one=m_userLayers.begin();one != m_userLayers.end();++one)
            {
                m_pPreMapNode->getMap()->removeLayer(one->second->GetModelLayer());
            }

            m_pCurMapNode->getMap()->beginUpdate();
            for(auto one=m_userLayers.begin();one != m_userLayers.end();++one)
            {
                m_pCurMapNode->getMap()->addLayer(one->second->GetModelLayer());
                one->second->UpdateMapNode();
            }
            m_pCurMapNode->getMap()->endUpdate();

            for(auto one=m_userLayers.begin();one != m_userLayers.end();++one)
            {
                one->second->GetModelLayer()->SetCanDelete(true);
            }
        }


        m_pGroup->removeChildren(0,m_pGroup->getNumChildren());
        /// 如果是三维地球
        if(m_bIs3D)
        {
            m_pGroup->getOrCreateStateSet()->setMode(GL_DEPTH_TEST,osg::StateAttribute::ON);
            m_pDepth->setWriteMask(true);
            if(!m_p3DRoot.valid())
            {
                m_p3DRoot = new osg::Group;
                Init3DLight();
                m_p3DRoot->addChild(m_pAtmosphere->GetNode());
            }

            if(!m_p3DRoot->containsNode(m_pCurMapNode))
            {
                m_p3DRoot->removeChildren(2,1);
                m_p3DRoot->addChild(m_pCurMapNode);
            }

            /// 添加到根节点
            m_pGroup->addChild(m_pSpaceEnv->AsOsgSceneNode()->GetRealNode());
            m_pGroup->addChild(m_p3DRoot);
        }
        else /// 如果是二维地球
        {
            dynamic_cast<IOsgViewPoint*>(m_pSceneGraph->GetMainWindow()->GetMainViewPoint())
                    ->GetOsgView()->getCamera()->setClearMask(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
            m_pGroup->getOrCreateStateSet()->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
            m_pDepth->setWriteMask(false);

            if(!m_pNoTran2DMapNode.valid())
            {
                m_pNoTran2DMapNode = new osg::Group;
            }

            if(!m_pNoTran2DMapNode->containsNode(m_pCurMapNode))
            {
                m_pNoTran2DMapNode->removeChildren(0,1);
                m_pNoTran2DMapNode->addChild(m_pCurMapNode);
            }

            m_pGroup->addChild(m_pNoTran2DMapNode);

            auto dWidth = m_pCurMapNode->getMap()->getProfile()->getExtent().width();
            if(!m_pLeftTran.valid())
            {
                m_pLeftTran = new osg::MatrixTransform;
            }

            if(!m_pLeftTran->containsNode(m_pCurMapNode))
            {
                m_pLeftTran->removeChildren(0,1);
                m_pLeftTran->addChild(m_pCurMapNode);
                m_pLeftTran->setMatrix(osg::Matrix::translate(osg::Vec3d(-dWidth,0.,0.)));
            }

            /// 如果右边的没有
            if(!m_pRightTran.valid())
            {
                m_pRightTran = new osg::MatrixTransform;
            }

            if(!m_pRightTran->containsNode(m_pCurMapNode))
            {
                m_pRightTran->removeChildren(0,1);
                m_pRightTran->addChild(m_pCurMapNode);
                m_pRightTran->setMatrix(osg::Matrix::translate(osg::Vec3d(dWidth,0.,0.)));
            }


            /// 添加到根节点
            m_pGroup->addChild(m_pRightTran);
            m_pGroup->addChild(m_pLeftTran);
        }

        ///切换视点
        IOsgViewPoint* pViewPoint = dynamic_cast<IOsgViewPoint*>(m_pSceneGraph->GetMainWindow()->GetMainViewPoint());
        if(nullptr != pViewPoint)
        {
            pViewPoint->ViewPointTypeChanged(m_bIs3D ? IOsgViewPoint::View_3DMap : IOsgViewPoint::View_2DMap);
        }

        m_bMapChanged=false;
    }

    

    /// 每一帧更新一次
    if(m_pCurMapNode.valid())
    {
        if(m_bIs3D)
        {
            static osgEarth::LogarithmicDepthBuffer s_logDepthBuffer;

            static osg::Vec3d eye,ignor;
            auto pEarthManipulator = dynamic_cast<osgEarth::EarthManipulator*>(m_pView->getCameraManipulator());

            /// 获取操作器的距离
            if(nullptr != pEarthManipulator)
            {
                if(pEarthManipulator->getViewpoint().range().value() < 22000)
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
        }

        if(m_bMouseMove)
        {
            ConvertCoord(m_nX,m_nY,m_stMousePos,0);
            m_bMouseMove=false;
        }
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
                m_bIs3D = true;
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
                }
            }
            else
            {
                m_extent = m_pCurMapNode->getMap()->getProfile()->getExtent();
                m_bIs3D=false;
            }
        }
        IOsgMapSceneNode::SetMapNode(m_pCurMapNode,m_pSceneGraph);
    }
}

/// 初始化光线
void CMap::Init3DLight()
{
    m_unStateSet |= PHONGLIGHTING_STATE;
    m_p3DRoot->setStateSet(m_pSceneGraph->ResouceLoader()->GetOrCreateStateSet(m_unStateSet));
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

    const osgEarth::Ellipsoid& rEllipsoid = m_pCurMapNode->getMapSRS()->getEllipsoid();
    float fInnerRadius = osg::minimum(rEllipsoid.getRadiusEquator(),rEllipsoid.getRadiusPolar());
    float fOuterRadius = fInnerRadius * 1.025;

    stateset->getOrCreateUniform("atmos_fInnerRadius", osg::Uniform::FLOAT)->set(fInnerRadius);
    stateset->getOrCreateUniform("atmos_fOuterRadius", osg::Uniform::FLOAT)->set(fOuterRadius);

    stateset = m_p3DRoot->getOrCreateStateSet();
    stateset->setDefine("OE_NUM_LIGHTS", "1");
    if(m_bOpenLight)
    {
        osgEarth::GLUtils::setLighting(m_pCurMapNode->getOrCreateStateSet(),osg::StateAttribute::ON);
    }

    m_p3DRoot->setStateSet(stateset);


    const Math::CVector& vSunPos = m_pSpaceEnv->GetSunPos();
    osg::Vec3 npos(vSunPos.GetX(),vSunPos.GetY(),vSunPos.GetZ());
    m_pLight->setPosition(osg::Vec4(npos,.0));
    m_pLightPosUniform->set(npos/npos.length());
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
