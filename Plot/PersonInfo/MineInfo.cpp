#include <ISceneCore.h>
#include <Plot/Map/IMapLocation.h>
#include <Plot/SceneShape/ILabel.h>
#include <Plot/SceneShape/IImage.h>
#include <Plot/Common/ISceneScreenGroup.h>
#include "MineInfo.h"

void CMineInfo::UpdateMapNode(osgEarth::MapNode *pMapNode)
{
    auto pLocation = dynamic_cast<IOsgMapSceneNode*>(m_pLocation);
    pLocation->UpdateMapNode(pMapNode);
}

void CMineInfo::InitNode()
{
    auto pScreenGroup = m_pSceneGraph->GetPlot()->CreateSceneGroup(SCREEN_GROUP)->AsSceneScreenGroup();
    m_pLabel = dynamic_cast<ILabel*>(m_pSceneGraph->GetPlot()->CreateSceneNode("ILabel"));
    m_pLocation = dynamic_cast<IMapLocation*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLocation"));
    m_pImage=dynamic_cast<IImage*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IImage"));

    TypeChanged();
    SceneImageSize size;
    size.unHeight=32;
    size.unWidth=32;
    size.bOutSet=true;
    m_pImage->SetImageSize(size);
    pScreenGroup->AddSceneNode(m_pImage);
    pScreenGroup->AddSceneNode(m_pLabel);
    m_pLocation->SetSceneNode(pScreenGroup);
    m_pLabel->SetFont("Fonts/msyh.ttf");

    ScenePixelOffset spOffset;
    spOffset.sHeight=0;
    spOffset.sWidth=16;
    m_pLabel->SetPixelOffset(spOffset);

    m_pRootNode = m_pLocation->AsOsgSceneNode()->GetOsgNode();
    m_preMask = m_pRootNode->getNodeMask();
}

void CMineInfo::PosChanged()
{
    m_pLocation->SetGeoPos(m_geoPos);
}

void CMineInfo::NameChanged()
{
    m_pLabel->SetText(m_sName);
}

void CMineInfo::ColorChanged()
{
    m_pImage->SetColor(m_stColor);
}

void CMineInfo::TypeChanged()
{
    const char* pFilePath=nullptr;
    switch (m_emType)
    {
    case MINE_EXPLOSION:
        pFilePath = "Image/Mine/17-2.png";
        break;
    case MINE_DISARM:
        pFilePath = "Image/Mine/17-1.png";
        break;
    default:
        pFilePath = "Image/Mine/17.png";
        break;
    }
    m_pImage->SetImagePath(pFilePath);
}
