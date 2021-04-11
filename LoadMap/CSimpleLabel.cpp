#include "CSimpleLabel.h"
#include <Plot/SceneShape/ILine.h>
#include <Plot/SceneShape/ILabel.h>
#include <SceneGraph/ISceneGraph.h>
#include <GisMath/GisMath.h>
#include <VersionMathCommon.h>
#include <Plot/Map/IMap.h>
#include <Plot/Map/IMapLine.h>
#include <Plot/Map/IMapLocation.h>
#include <Plot/Common/ISceneCameraGroup.h>
#include <Plot/Common/ISceneScreenGroup.h>
CSimpleLabel::CSimpleLabel(ISceneGraph* p)
{
    m_pSceneGraph = p;
    m_pLabel =dynamic_cast<ILabel*>(m_pSceneGraph->GetPlot()->CreateSceneNode("ILabel"));
    m_pLine  = dynamic_cast<ILine*>(m_pSceneGraph->GetPlot()->CreateSceneNode("ILine"));
    m_pRoot  = dynamic_cast<IMapLocation*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLocation"));
    //ISceneScreenGroup* pCameraGroup   =   m_pSceneGraph->GetPlot()->CreateSceneGroup(SCREEN_GROUP)->AsSceneScreenGroup();
    //pCameraGroup->SetAvoid(false);
     m_pSceneGroup=  m_pSceneGraph->GetPlot()->CreateSceneGroup(STANDARD_GROUP);
     ISceneCameraGroup* pCameraGroup   =   m_pSceneGraph->GetPlot()->CreateSceneGroup(CAMERA_GROUP)->AsSceneCameraGroup();
     //pCameraGroup->SetRenderIndex(100);

//    ScenePixelOffset offset;
//    offset.sHeight =0;
//    offset.sWidth = 10;
//    m_pLabel->SetPixelOffset(offset);

    m_pLine->SetLineWidth(2);
    pCameraGroup->AddSceneNode(m_pLabel);
    m_pSceneGroup->AddSceneNode(m_pLine);

    m_pSceneGroup->AddSceneNode(pCameraGroup);

    m_pRoot->SetSceneNode(m_pSceneGroup);
    m_pTarget = nullptr;
    m_bLabelChange = true;
    m_bSimpleLabelShow = true;
    m_Color.fG = m_Color.fB = 0.0;
    m_pSceneGroup->SetCanPick(true);

    m_pLabel->SetFontFillColor(m_Color);
    m_pLine->SetColor(m_Color);
    m_TX = 0;
    m_TY = 0;
    m_TZ = 0;

    m_bSelect = false;
}
CSimpleLabel::~CSimpleLabel()
{

}

void CSimpleLabel::MouseMove(MouseButtonMask, int x, int y)
{
    ScenePos mapPos;
//    m_pSceneGraph->GetMap()->ConvertCoord(x,y,mapPos,0);
    double dX,dY,dZ;
    GisMath::LBH2XYZ(mapPos.fX*DD2R,mapPos.fY*DD2R,mapPos.fZ-1000,dX,dY,dZ);

    ScenePos pox;
    pox.fX = dX;
    pox.fY = dY;
    pox.fZ = dZ;
    //m_pRoot->SetGeoPos(mapPos);
    m_pLabel->SetPos(pox);
    m_pLine->UpdatePoint(1,pox);
    float  fLon , fLat, fHeight;
//    m_pTarget->GetPosition(&fLon,&fLat,&fHeight);

    double dx,dy,dz;
    GisMath::LBH2XYZ(fLon*DD2R,fLat*DD2R,fHeight, dx,dy,dz);

    m_TX = dX - dx ;
    m_TY = dY - dy;
    m_TZ = dZ -dz;

}

IMapLocation* CSimpleLabel::getNode()
{
    return  m_pRoot;
}
void CSimpleLabel::setSimpleLabel(const char* pLabel)
{
    simpleLabel = std::string(pLabel);
    m_CurLabel = simpleLabel;

    m_bLabelChange = true;
}
void CSimpleLabel::setComplexLabel(const char* pLabel)
{
    fullLabel = std::string(pLabel);

    m_CurLabel = fullLabel;

    m_bLabelChange = true;
}
void CSimpleLabel::SetTarget(IPlotBase* pTarget)
{
    pTarget =pTarget;
    //
    ScenePos geoPos;
//    pTarget->GetPosition(&geoPos.fLon,&geoPos.fLat,&geoPos.fHeight);

    double x,y,z;

    GisMath::LBH2XYZ(geoPos.fX*DD2R,geoPos.fY*DD2R,geoPos.fZ,x,y,z);

    m_TargetPos = geoPos;

    ScenePos pos1;
    pos1.fX = x;
    pos1.fY = y;
    pos1.fZ = z;

    ScenePos pos;
    pos.fX = x +m_TX;
    pos.fY = y +m_TY;
    pos.fZ = z +m_TZ+1000000;
    m_pLabel->SetPos(pos);

    double  fLon , fLat, fHeight;
    GisMath::XYZ2LBH(pos.fX,pos.fY,pos.fZ,fLon,fLat,fHeight);

//    MapGeoPos tPos;
//    tPos.fHeight = fHeight;
//    tPos.fLat = fLat*DR2D;
//    tPos.fLon = fLon*DR2D;
    if(m_pLine->GetMulPos().size() ==0)
    {
        m_pLine->AddPoint(0,pos1);
        m_pLine->AddPoint(1,pos);

    }
    else
    {
         m_pLine->UpdatePoint(0,pos1);
         m_pLine->UpdatePoint(1,pos);

    }
    m_pTarget = pTarget;
}
void CSimpleLabel::setColor(long uColor)
{
    if(m_LabelColor!= uColor)
    {
        m_LabelColor= uColor;
        SceneColor color;
//        color.fR = GetRValue(m_LabelColor);
//        color.fG = GetGValue(m_LabelColor);
//        color.fB = GetBValue(m_LabelColor);

        m_pLabel->SetFontFillColor(color);
    }
}

bool CSimpleLabel::JudgeID(int unSelected,unsigned int type)
{
    int nnID= m_pSceneGroup->PickID() ;
    if(type == 1 && !m_bSelect &&  m_pSceneGroup->PickID() == unSelected)
    {
        m_bSelect = true;
        m_pSceneGraph->GetMainWindow()->SubMessage(this);
        return true;
    }
    if(type == 1 && m_bSelect)
    {
        m_bSelect = false;
        m_pSceneGraph->GetMainWindow()->UnSubMessage(this);
        return true;
    }
    else if(type == 2 &&  m_pSceneGroup->PickID() == unSelected)
    {
        m_pSceneGraph->GetMainWindow()->UnSubMessage(this);
        m_bSelect = false;
        if(m_bSimpleLabelShow)
            m_CurLabel = fullLabel;
        else
            m_CurLabel = simpleLabel;

        m_bSimpleLabelShow= !m_bSimpleLabelShow;
        m_bLabelChange = true;

    }
    return false;
}
void CSimpleLabel::update()
{
    if(m_pTarget == nullptr || m_bSelect)
        return;

    if(m_bLabelChange)
    {
        m_pLabel->SetText(m_CurLabel);
        m_bLabelChange= false;
    }
   ScenePos geoPos;
//   m_pTarget->GetPosition(&geoPos.fLon,&geoPos.fLat,&geoPos.fHeight);

    if(m_TargetPos != geoPos) //目标移动
    {

        ScenePos tmPos1,tmPos2;

        double x,y,z;
        GisMath::LBH2XYZ(geoPos.fX*DD2R,geoPos.fY*DD2R,geoPos.fZ, x,y,z);

        tmPos1.fX = x;
        tmPos1.fY = y;
        tmPos1.fZ = z;
        tmPos2.fX = x + m_TX;
        tmPos2.fY = y + m_TY;
        tmPos2.fZ = z + m_TZ;

        m_pLabel->SetPos(tmPos2);
        m_pLine->UpdatePoint(0,tmPos1);
        m_pLine->UpdatePoint(1,tmPos2);
    }

}
