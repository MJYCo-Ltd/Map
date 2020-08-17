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

#include "PlotManager.h"
#include <Inner/IRender.h>
#include <Inner/ILoadResource.h>
#include <Inner/IOsgSceneNode.h>
#include <Inner/IOsgViewPoint.h>
#include <Inner/OsgExtern/OsgExtern.h>
#include "SpaceEnv.h"
#include "Map.h"

CMap::CMap(MapType type, ISceneGraph *pSceneGraph):
    QtOsgSceneNode<IMap>(pSceneGraph),
    m_emType(type)
{
    m_pPlotManager = new CPlotManager(m_pSceneGraph);
    m_pSpaceEnv = new CSpaceEnv(m_pSceneGraph);
}

CMap::~CMap()
{
    delete m_pPlotManager;

    if(m_pCamera.valid())
    {
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new RemoveFromeScene(m_pCamera));
        m_pCamera = nullptr;
    }

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
}

void CMap::SubMessage(IMapMessageObserver *pMsgObr)
{
    auto findResult = find(m_listObserver.begin(),m_listObserver.end(),pMsgObr);
    if(m_listObserver.end() == findResult)
    {
        m_listObserver.push_back(pMsgObr);
    }
}

void CMap::UnSubMessage(IMapMessageObserver *pMsgObr)
{
    auto findResult = find(m_listObserver.begin(),m_listObserver.end(),pMsgObr);
    if(findResult != m_listObserver.end())
    {
        m_listObserver.erase(findResult);
    }
}

/// 坐标转换
bool CMap::ConvertCoord(int &nX, int &nY, ScenePos &geoPos, short TranType)
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

        switch (m_emType)
        {
        case MAP_2D:
            if(m_pMap2DNode->getTerrain()->getWorldCoordsUnderMouse(pOsgViewPoint->GetOsgView(), nX, nY, world))
            {
                geoPoint.fromWorld(m_pMap2DNode->getMapSRS(),world);
                geoPoint.makeGeographic();
                geoPos.bIsGeo = true;
                geoPos.fLon = geoPoint.x();
                geoPos.fLat = geoPoint.y();
                geoPos.fHeight = geoPoint.z();
                return (true);
            }
            break;
        case MAP_3D:
            if(m_pMap3DNode->getTerrain()->getWorldCoordsUnderMouse(pOsgViewPoint->GetOsgView(), nX, nY, world))
            {
                geoPoint.fromWorld(m_pMap3DNode->getMapSRS(),world);
                geoPoint.makeGeographic();
                geoPos.bIsGeo = true;
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
const MapLayers &CMap::GetMapLayers()
{
    return(m_allLayers);
}

/// 控制图层显隐
void CMap::SetLayerVisible(const string & sLayerName)
{
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

/// 获取标绘管理类
IPlotManager *CMap::GetPlotManager()
{
    return(m_pPlotManager);
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
void CMap::InitSceneNode()
{
    m_pOsgNode = new osg::Group;

    m_pPlotManager->RegisterPlotType();

    osg::Camera* pCamera = dynamic_cast<IOsgViewPoint*>(m_pSceneGraph->GetMainWindow()->GetMainViewPoint())
            ->GetOsgView()->getCamera();
    osgEarth::GLUtils::setGlobalDefaults(pCamera->getOrCreateStateSet());
    m_pSpaceEnv->SetMainCamara(pCamera);
    InitMap();
}

class RotateCallBack:public osg::Callback
{
public:
    RotateCallBack(osg::MatrixTransform* pMatTrans):m_pPlaceNode(pMatTrans)
    {
    }

    bool run(osg::Object* object, osg::Object* data)
    {
        static double nAngle(0);
        nAngle += 0.001;

        m_pPlaceNode->setMatrix(osg::Matrix::rotate(nAngle,osg::Z_AXIS));
        return traverse(object, data);
    }

protected:
    virtual ~RotateCallBack() {}
private:

    osg::observer_ptr<osg::MatrixTransform> m_pPlaceNode;
};

void CMap::InitMap()
{
    m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CClearChildNode(m_pOsgNode));

    switch (m_emType)
    {
    case MAP_2D:
    {
        if(!m_p2DRoot.valid())
        {
            auto node = m_pSceneGraph->ResouceLoader()->LoadNode("Projected_image.earth");
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
        }

        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CModifyNode(m_pOsgNode,m_p2DRoot,true));
        m_pPlotManager->UpdateMapNode(m_pMap3DNode,m_pMap2DNode);
        m_pSpaceEnv->ShowSpaceBackGround(false);
    }
        break;
    case MAP_3D:
    {
        if(!m_pMap3DNode.valid())
        {
            auto node = m_pSceneGraph->ResouceLoader()->LoadNode("Geocentric.earth");
            m_pMap3DNode = osgEarth::MapNode::findMapNode(node);
#if OSGEARTH_VERSION_GREATER_OR_EQUAL(3,0,0)
            m_pMap3DNode->open();
#endif

            m_pCamera = new osg::Camera;
            m_pCamera->setClearMask(GL_DEPTH_BUFFER_BIT);
            m_pCamera->setRenderOrder( osg::Camera::NESTED_RENDER);
            m_pCamera->setAllowEventFocus(false);
            m_pCamera->setProjectionResizePolicy(osg::Camera::FIXED);
            m_pCamera->setComputeNearFarMode( osg::CullSettings::COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES );
            m_pCamera->addChild(node);
            m_pCamera->setGraphicsContext(dynamic_cast<IOsgViewPoint*>(m_pSceneGraph->GetMainWindow()->GetMainViewPoint())
                                          ->GetOsgView()->getCamera()->getGraphicsContext());

            m_pCamera->addCullCallback(new osgEarth::AutoClipPlaneCullCallback(m_pMap3DNode));

            osgEarth::Util::LogarithmicDepthBuffer buffer;
            buffer.setUseFragDepth( true );
            buffer.install(m_pCamera);
        }

        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CModifyNode(m_pOsgNode.get(),m_pCamera.get(),true));
        m_pPlotManager->UpdateMapNode(m_pMap2DNode,m_pMap3DNode);
        m_pSpaceEnv->InitSceneNode();
        AddSceneNode(m_pSpaceEnv);
        m_pSpaceEnv->ShowSpaceBackGround(true);
    }
        break;
    }
}

IMap* CreateMap(MapType emType, ISceneGraph* pSceneGraph)
{
    return(new CMap(emType,pSceneGraph));
}
