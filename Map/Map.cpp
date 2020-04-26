#include <osg/MatrixTransform>
#include <osgEarth/Version>

#if OSGEARTH_VERSION_GREATER_OR_EQUAL(3,0,0)
#include <osgEarth/LogarithmicDepthBuffer>
#else
#include <osgEarthUtil/LogarithmicDepthBuffer>
#endif

#include <osgEarth/GLUtils>

#include <QLibrary>

#include <SpaceEnv/ISpaceEnv.h>
#include "PlotManager.h"
#include "Inner/ILoadResource.h"
#include "Map.h"

CMap::CMap(MapType type, ISceneGraph *pSceneGraph):
    QtSceneNode<IMap>(pSceneGraph),
    m_emType(type)
{
}

CMap::~CMap()
{
    delete m_pPlotManager;
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
void CMap::SetLayerVisible(const string &sLayerName)
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
    QtSceneNode<IMap>::InitSceneNode();
    m_pPlotManager = new CPlotManager(m_pSceneGraph);
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
            auto node = m_pSceneGraph->ResouceLoader()->LoadNode("Projected.earth");
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
        if(!m_pCamera.valid())
        {
            auto node = m_pSceneGraph->ResouceLoader()->LoadNode("Geocentric.earth");
            m_pMap3DNode = osgEarth::MapNode::findMapNode(node);
#if OSGEARTH_VERSION_GREATER_OR_EQUAL(3,0,0)
            m_pMap3DNode->open();
#endif

            m_pCamera = new osg::Camera;
            m_pCamera->setClearMask(GL_DEPTH_BUFFER_BIT);
            m_pCamera->setRenderOrder( osg::Camera::NESTED_RENDER );
            m_pCamera->setComputeNearFarMode( osg::CullSettings::COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES );
            m_pCamera->addChild(node);

            osgEarth::Util::LogarithmicDepthBuffer buffer;
            buffer.setUseFragDepth( true );
            buffer.install(m_pCamera);

            osgEarth::GLUtils::setGlobalDefaults(m_pCamera->getOrCreateStateSet());
        }

        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CModifyNode(m_pOsgNode.get(),m_pCamera.get(),true));
        m_pPlotManager->UpdateMapNode(m_pMap2DNode,m_pMap3DNode);
    }
        break;
    }

    /// 加载星空
//    LoadSpaceEnv();
}

/// 加载星空背景
void CMap::LoadSpaceEnv()
{
    if(nullptr == m_pSpaceEnv)
    {
        typedef ISpaceEnv* (*CreateSpaceEnvFun)(ISceneGraph*);
#ifdef _DEBUG
        QLibrary loadMap("SpaceEnvd.dll");
#else
        QLibrary loadMap("SpaceEnv.dll");
#endif
        if(loadMap.load())
        {
            CreateSpaceEnvFun pCreateSpaceEnv = reinterpret_cast<CreateSpaceEnvFun>(loadMap.resolve("CreateSpaceEnv"));
            if(nullptr != pCreateSpaceEnv)
            {
                m_pSpaceEnv = pCreateSpaceEnv(m_pSceneGraph);
                m_pSpaceEnv->InitSceneNode();
            }
        }
    }

    if(nullptr != m_pSpaceEnv && MAP_3D == m_emType)
    {
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CModifyNode(m_pOsgNode.get(),m_pSpaceEnv->GetOsgNode(),true));
    }
}

IMap* CreateMap(MapType emType, ISceneGraph* pSceneGraph)
{
    return(new CMap(emType,pSceneGraph));
}
