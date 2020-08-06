#include <osgViewer/View>
#include <osg/MatrixTransform>
#include <osgEarth/Version>

#if OSGEARTH_VERSION_GREATER_OR_EQUAL(3,0,0)
#include <osgEarth/LogarithmicDepthBuffer>
#else
#include <osgEarthUtil/LogarithmicDepthBuffer>
#endif

#include <osgEarth/GLUtils>
#include <osgEarth/AutoClipPlaneHandler>

#include <QLibrary>

#include <SpaceEnv/ISpaceEnv.h>
#include "PlotManager.h"
#include <Inner/IRender.h>
#include <Inner/ILoadResource.h>
#include <Inner/IOsgSceneNode.h>
#include <Inner/IOsgViewPoint.h>
#include <Inner/OsgExtern/OsgExtern.h>
#include "Map.h"

CMap::CMap(MapType type, ISceneGraph *pSceneGraph):
    QtOsgSceneNode<IMap>(pSceneGraph),
    m_emType(type)
{
}

CMap::~CMap()
{
    delete m_pPlotManager;

    m_pCamera = nullptr;
    m_p2DRoot = nullptr;
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
    return(true);
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

/// 获取星空背景
ISpaceEnv *CMap::GetSpaceEnv()
{
    return(m_pSpaceEnv);
}

/// 初始化场景
void CMap::InitSceneNode()
{
    QtOsgSceneNode<IMap>::InitSceneNode();
    m_pPlotManager = new CPlotManager(m_pSceneGraph);

    osg::Camera* pCamera = dynamic_cast<IOsgViewPoint*>(m_pSceneGraph->GetMainWindow()->GetMainViewPoint())
            ->GetOsgView()->getCamera();
    osgEarth::GLUtils::setGlobalDefaults(pCamera->getOrCreateStateSet());
    InitMap();
}

void CMap::InitMap()
{
    m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CClearChildNode(m_pOsgNode));

    switch (m_emType)
    {
    case MAP_2D:
    {
        if(!m_p2DRoot.valid())
        {
            auto node = m_pSceneGraph->ResouceLoader()->LoadNode("Projected_ok.earth");
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
    }
        break;
    }

    /// 加载星空
    LoadSpaceEnv();
}

/// 加载星空背景
void CMap::LoadSpaceEnv()
{
    if(nullptr == m_pSpaceEnv)
    {
        typedef ISpaceEnv* (*CreateSpaceEnvFun)(ISceneGraph*);
#ifdef Q_OS_WIN

#ifdef QT_NO_DEBUG
        QLibrary loadMap("SpaceEnv.dll");
#else
        QLibrary loadMap("SpaceEnvd.dll");
#endif

#else

#ifdef QT_NO_DEBUG
        QLibrary loadMap("libSpaceEnv.so");
#else
        QLibrary loadMap("libSpaceEnvd.so");
#endif

#endif
        if(loadMap.load())
        {
            CreateSpaceEnvFun pCreateSpaceEnv = reinterpret_cast<CreateSpaceEnvFun>(loadMap.resolve("CreateSpaceEnv"));
            if(nullptr != pCreateSpaceEnv)
            {
                m_pSpaceEnv = pCreateSpaceEnv(m_pSceneGraph);
                dynamic_cast<IOsgSceneNode*>(m_pSpaceEnv)->InitSceneNode();
            }
        }
    }

    if(nullptr != m_pSpaceEnv)
    {
        osg::Camera* pCamera = dynamic_cast<IOsgViewPoint*>(m_pSceneGraph->GetMainWindow()->GetMainViewPoint())
                ->GetOsgView()->getCamera();
        if(MAP_3D == m_emType)
        {
            pCamera->setClearMask(GL_DEPTH_BUFFER_BIT);
            pCamera->setClearColor(osg::Vec4(0.0,0.0,0.0,1.0));
            m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CModifyNode(m_pOsgNode.get(),
                                                                                  dynamic_cast<IOsgSceneNode*>(m_pSpaceEnv)->GetOsgNode(),true));
        }
        else if(MAP_2D == m_emType)
        {
            pCamera->setClearMask(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
            pCamera->setClearColor(osg::Vec4(0.63922,0.8,1.0,1.0));
        }
    }
}

IMap* CreateMap(MapType emType, ISceneGraph* pSceneGraph)
{
    return(new CMap(emType,pSceneGraph));
}
