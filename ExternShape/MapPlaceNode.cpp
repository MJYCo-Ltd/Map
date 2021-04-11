#include <ISceneCore.h>
#include <Plot/Map/IMapLocation.h>
#include <Plot/SceneShape/ILabel.h>
#include <Plot/SceneShape/IImage.h>
#include <Plot/Common/ISceneScreenGroup.h>
#include <ExternShape/MapPlaceNode.h>


CPlaceNode::CPlaceNode(ISceneGraph *pSceneGraph):m_pSceneGraph(pSceneGraph)
{
    InitNode();
}

void CPlaceNode::SetPos(float fLon, float fLat)
{
    ScenePos pos;
    pos.fX = fLon;
    pos.fY = fLat;
    m_pLocation->SetGeoPos(pos);
}

const ScenePos CPlaceNode::GetPos()
{
    return(m_pLocation->GeoPos());
}

void CPlaceNode::SetText(const std::string &sTextInfo)
{
    m_pLabel->SetText(sTextInfo);
}


void CPlaceNode::SetColor(const SceneColor &rColor)
{
    if(nullptr != m_pImage) m_pImage->SetColor(rColor);
}

void CPlaceNode::SetImagePath(const std::string &sPath)
{
    if(nullptr == m_pImage)
    {
        m_pImage=dynamic_cast<IImage*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IImage"));
    }
    SceneImageSize size;
    size.unHeight=32;
    size.unWidth=32;
    size.bOutSet=true;
    m_pImage->SetImageSize(size);
    m_pSceneScreenGroup->AddSceneNode(m_pImage);
    ScenePixelOffset spOffset;
    spOffset.sHeight=0;
    spOffset.sWidth=16;
    m_pLabel->SetPixelOffset(spOffset);
    m_pImage->SetImagePath(sPath);
}

void CPlaceNode::SetImageSize(const SceneImageSize &stSize)
{   
    if(nullptr == m_pImage) return;

    m_pImage->SetImageSize(stSize);
    ScenePixelOffset spOffset;
    spOffset.sHeight=0;
    spOffset.sWidth=stSize.unWidth/2;
    m_pLabel->SetPixelOffset(spOffset);
}

void CPlaceNode::InitNode()
{
    m_pSceneScreenGroup = m_pSceneGraph->GetPlot()->CreateSceneGroup(SCREEN_GROUP)->AsSceneScreenGroup();
    m_pLabel = dynamic_cast<ILabel*>(m_pSceneGraph->GetPlot()->CreateSceneNode("ILabel"));
    m_pLocation = dynamic_cast<IMapLocation*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLocation"));
    m_pSceneScreenGroup->AddSceneNode(m_pLabel);
    m_pLocation->SetSceneNode(m_pSceneScreenGroup);
    m_pLabel->SetFont("Fonts/msyh.ttf");
}
