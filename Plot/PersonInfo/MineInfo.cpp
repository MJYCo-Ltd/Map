#include <ISceneCore.h>
#include <Plot/Map/IMapLocation.h>
#include <Plot/SceneShape/ILabel.h>
#include <Plot/SceneShape/IImage.h>
#include <SceneGraph/ISceneGroup.h>
#include "MineInfo.h"

void CMineInfo::UpdateMapNode(osgEarth::MapNode *pMapNode)
{
    auto pLocation = dynamic_cast<IOsgMapSceneNode*>(m_pLocation);
    pLocation->UpdateMapNode(pMapNode);
}

void CMineInfo::InitNode()
{
    m_pLabel = dynamic_cast<ILabel*>(m_pSceneGraph->GetPlot()->CreateSceneNode("ILabel"));
    m_pLocation = dynamic_cast<IMapLocation*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLocation"));
    m_pImage=dynamic_cast<IImage*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IImage"));

    m_pImage->SetImagePath("Image/Mine/17.png");
    SceneImageSize size;
    size.unHeight=32;
    size.unWidth=32;
    size.bOutSet=true;
    m_pImage->SetImageSize(size);
    m_pLocation->SetSceneNode(m_pImage);
    m_pLabel->SetAttachNode(m_pImage);
    m_pLabel->SetFont("Fonts/msyh.ttf");

    ScenePixelOffset spOffset;
    spOffset.sHeight=0;
    spOffset.sWidth=16;
    m_pLabel->SetPixelOffset(spOffset);

    m_pRootNode = m_pLocation->AsOsgSceneNode()->GetOsgNode();
    m_preMask = m_pRootNode->getNodeMask();

    osgEarth::ScreenSpaceLayout::activate(m_pRootNode->getOrCreateStateSet());
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
