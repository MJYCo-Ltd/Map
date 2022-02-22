#include <ISceneCore.h>
#include <Plot/Map/IMapLocation.h>
#include <Plot/SceneShape/ILabel.h>
#include <Plot/SceneShape/IImage.h>
#include <Plot/Common/ISceneScreenGroup.h>
#include <ExternShape/MapPlaceNode.h>
#include <Plot/Common/ISceneAttitudeGroup.h>

CPlaceNode::CPlaceNode(ISceneGraph *pSceneGraph):m_pSceneGraph(pSceneGraph)
{
    InitNode();
}

void CPlaceNode::SetPos(double fLon, double fLat,double dHeight)
{
    ScenePos pos;
    pos.dX = fLon;
    pos.dY = fLat;
    pos.dZ = dHeight;
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

void CPlaceNode::SetAttitude(const SceneAttitude & attitude)
{
    m_pAttitudeGroup->SetAttitude(attitude);
}

void CPlaceNode::SetColor(const SceneColor &rColor)
{
    if(nullptr == m_pImage)
    {
        CreateImage();
    }
    //m_pLabel->SetFontFillColor(rColor);
    m_pImage->SetColor(rColor);
}

void CPlaceNode::SetQImage(const QImage & image)
{
    if(nullptr == m_pImage)
    {
        CreateImage();
    }

    m_pImage->SetQImage(image);
}

void CPlaceNode::SetImagePath(const std::string &sPath)
{
    if(nullptr == m_pImage)
    {
        CreateImage();
    }

    m_pImage->SetImagePath(sPath);
}

void CPlaceNode::SetImageSize(const SceneImageSize &stSize)
{   
    if(nullptr == m_pImage)
    {
        CreateImage();
    }

    m_pImage->SetImageSize(stSize);
    ScenePixelOffset spOffset;
    spOffset.sHeight=0;
    spOffset.sWidth=stSize.unWidth/2;
    //m_pLabel->SetPixelOffset(spOffset);
}

void CPlaceNode::InitNode()
{
    m_pAttitudeGroup = m_pSceneGraph->GetPlot()->CreateSceneGroup(ATTITUDE_GROUP)->AsSceneAttitudeGroup();
    m_pSceneScreenGroup = m_pSceneGraph->GetPlot()->CreateSceneGroup(SCREEN_GROUP)->AsSceneScreenGroup();
    m_pLabel = dynamic_cast<ILabel*>(m_pSceneGraph->GetPlot()->CreateSceneNode("ILabel"));
    m_pLocation = dynamic_cast<IMapLocation*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLocation"));
    m_pSceneScreenGroup->AddSceneNode(m_pLabel);
    m_pLabel->SetFontSize(14);
    m_pAttitudeGroup->AddSceneNode(m_pSceneScreenGroup);
    m_pLocation->SetSceneNode(m_pAttitudeGroup);
    m_pLabel->SetFont("Fonts/msyh.ttf");
}

void CPlaceNode::CreateImage()
{
    m_pImage=dynamic_cast<IImage*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IImage"));
    m_pImage->OpenCull(false);
    SceneImageSize size;
    size.unHeight=32;
    size.unWidth=32;
    size.bOutSet=true;
    m_pImage->SetImageSize(size);
    m_pImage->AlwasOnTop(true);
    m_pAttitudeGroup->AddSceneNode(m_pImage);
    ScenePixelOffset spOffset;
    spOffset.sHeight=0;
    spOffset.sWidth=16;
    //m_pLabel->SetPixelOffset(spOffset);
}
