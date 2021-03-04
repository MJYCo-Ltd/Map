#include <Inner/ImplSceneAttitudeGroup.hpp>
#include <Inner/ImplSceneFlashGroup.hpp>
#include <Inner/ImplSceneLodGroup.hpp>
#include <Inner/ImplSceneScaleGroup.hpp>
#include <Inner/ImplSceneCameraGroup.hpp>
#include <Inner/ImplSceneScreenGroup.hpp>
#include <Inner/ImplSceneModel.hpp>

#include <Plot/Common/ISceneAttitudeGroup.h>
#include <Plot/Common/ISceneFlashGroup.h>
#include <Plot/Common/ISceneLodGroup.h>
#include <Plot/Common/ISceneScaleGroup.h>
#include <Plot/Common/ISceneCameraGroup.h>
#include <Plot/Common/ISceneScreenGroup.h>
#include <Plot/Common/ISceneModel.h>

#include "MapNodeFactory.h"
#include "PlotManager.h"


CPlotManager::CPlotManager(ISceneGraph* pSceneGraph):
    IPlot(pSceneGraph)
{
    m_pCreateFactory = new CMapNodeFactory(m_pSceneGraph);
}

CPlotManager::~CPlotManager()
{
}

/// 根据类型创建节点
ISceneGroup *CPlotManager::CreateSceneGroup(SCENE_GROUP_TYPE emType)
{
    ISceneGroup *pGroup(nullptr);

    switch(emType)
    {
    case ATTITUDE_GROUP:
        pGroup = new ImplSceneAttitudeGroup<ISceneAttitudeGroup>(m_pSceneGraph);
        break;
    case FLASH_GROUP:
        pGroup = new ImplSceneFlashGroup<ISceneFlashGroup>(m_pSceneGraph);
        break;
    case LOD_GROUP:
        pGroup = new ImplSceneLodGroup<ISceneLodGroup>(m_pSceneGraph);
        break;
    case SCALE_GROUP:
        pGroup = new ImplSceneScaleGroup<ISceneScaleGroup>(m_pSceneGraph);
        break;
    case CAMERA_GROUP:
        pGroup = new ImplSceneCameraGroup<ISceneCameraGroup>(m_pSceneGraph);
        break;
    case SCREEN_GROUP:
        pGroup = new ImplSceneScreenGroup<ISceneScreenGroup>(m_pSceneGraph);
        break;
    default:
        pGroup = new ImplSceneGroup<ISceneGroup>(m_pSceneGraph);
        break;
    }

    if(nullptr != pGroup)
    {
        pGroup->AsOsgSceneNode()->Init();
        m_pCreateFactory->InsertNode(pGroup);
    }

    return(pGroup);
}



/// 根据接口类型创建地图节点
ISceneNode *CPlotManager::CreateSceneNode(const std::string& csInterfaceName)
{
    return(m_pCreateFactory->CreateSceneNode(csInterfaceName));
}

/// 加载模型
ISceneNode *CPlotManager::LoadSceneNode(const std::string &sPath, bool bRef)
{
    osg::Node* pOsgNode = m_pSceneGraph->ResouceLoader()->LoadNode(sPath,bRef);
    if(nullptr != pOsgNode)
    {
        auto pNode = new ImplSceneModel<ISceneModel>(m_pSceneGraph);
        pNode->Init();
        pNode->SetModel(pOsgNode);
        m_pCreateFactory->InsertNode(pNode);
        return(pNode);
    }

    return(nullptr);
}

/// 强制删除没用的地图节点
void CPlotManager::FreeNoUseSceneNode()
{
    m_pCreateFactory->DeleteNoUseSceneNode();
}

/// 注册可以标绘的类型
void CPlotManager::RegisterPlotType()
{
    m_pCreateFactory->InitFactory();
}

IPlot *CreatePlot(ISceneGraph *pSceneGraph)
{
    return(new CPlotManager(pSceneGraph));
}
