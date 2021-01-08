#include <QSet>
#include <ISceneCore.h>
#include <Inner/ILoadResource.h>
#include <Inner/OsgExtern/OsgExtern.h>
#include "PersonInfo.h"

static const char s_sPersonInfo[]="IPersonInfo";
static const char s_sMineInfo[]="IMineInfo";

CPersonInfo::~CPersonInfo()
{
}

void CPersonInfo::UpdateMapNode(osgEarth::MapNode *pMapNode)
{
    m_pPerson->setMapNode(pMapNode);
    if(pMapNode->isGeocentric())
    {
        m_pPerson->setHorizonCulling(true);
    }
}

/// 初始化节点
void CPersonInfo::InitNode()
{
    ImplMapSceneNode<IPersonInfo>::InitNode();


    m_placeStyle.getOrCreateSymbol<osgEarth::TextSymbol>()->encoding() = osgEarth::TextSymbol::ENCODING_UTF8;
    m_placeStyle.getOrCreateSymbol<osgEarth::TextSymbol>()->alignment() = osgEarth::TextSymbol::ALIGN_LEFT_CENTER;
    m_placeStyle.getOrCreateSymbol<osgEarth::TextSymbol>()->size() = 20;
    std::string sFontPath = GetExePath();
    sFontPath += "fonts/msyh.ttf";
    m_placeStyle.getOrCreateSymbol<osgEarth::TextSymbol>()->font() = sFontPath;
    m_placeStyle.getOrCreateSymbol<osgEarth::TextSymbol>()->fill()->color() = osgEarth::Color::White;
    m_placeStyle.getOrCreateSymbol<osgEarth::TextSymbol>()->halo()->color() = osgEarth::Color::Black;
    m_placeStyle.getOrCreate<osgEarth::AltitudeSymbol>()->clamping() =
            osgEarth::AltitudeSymbol::CLAMP_RELATIVE_TO_TERRAIN;
    m_placeStyle.getOrCreate<osgEarth::IconSymbol>()->declutter() = false;
    m_placeStyle.getOrCreate<osgEarth::IconSymbol>()->alignment() = osgEarth::IconSymbol::ALIGN_RIGHT_CENTER;

    std::string sImagePath = "ico/red.png";
    auto pImage = m_pSceneGraph->ResouceLoader()->LoadImage(sImagePath,32,32);


    m_pPerson = new osgEarth::PlaceNode(osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"),0,0,0),
                                        m_sName,m_placeStyle,pImage);
    m_pPerson->setDynamic(true);
    SetOsgNode(m_pPerson.get());
}

/// 位置更新消息
void CPersonInfo::PosChanged()
{
    m_bPosChanged=true;
    NodeChanged();
}

///名称更新
void CPersonInfo::NameChanged()
{
    m_bNameChanged = true;
    NodeChanged();
}

///分组更新
void CPersonInfo::GroupChanged()
{
    m_bGroupChanged=true;
    NodeChanged();
}

///状态更新
void CPersonInfo::StatusChanged()
{
    m_bStatusChanged=true;
    NodeChanged();
}

///更新节点
void CPersonInfo::UpdateNode()
{
    if(m_bPosChanged)
    {
        m_pPerson->setPosition(osgEarth::GeoPoint(
                                   osgEarth::SpatialReference::get("wgs84"),
                                   m_geoPos.fLon,m_geoPos.fLat,m_geoPos.fHeight));
        m_bPosChanged=false;
    }

    if(m_bNameChanged)
    {
        m_pPerson->setText(m_sName);
        m_bNameChanged=false;
    }

    if(m_bGroupChanged)
    {
        changeImage();
        m_bGroupChanged=false;
    }

    if(m_bStatusChanged)
    {
        changeImage();
        m_bStatusChanged=false;
    }

    ImplMapSceneNode<IPersonInfo>::UpdateNode();
}

/// 更新图片
void CPersonInfo::changeImage()
{
    std::string sIconPath="ico/";
    switch(m_emGroupType)
    {
    case RED_GROUP:
        sIconPath += "red";
        break;
    case BLUE_GROUP:
        sIconPath += "blue";
        break;
    default:
        sIconPath += "white";
        break;
    }

    switch (m_emPersonStatus)
    {
    case PERSON_HURT:
        sIconPath += "_hurt";
        break;
    case PERSON_HIT:
        sIconPath += "_fire";
        break;
    case PERSON_DATH:
        sIconPath += "_death";
        break;
    case PERSON_UNLINE:
        sIconPath += "_unline";
        break;
    default:
        break;
    }

    sIconPath += ".png";
    m_pPerson->setIconImage(m_pSceneGraph->ResouceLoader()->LoadImage(sIconPath,32,32));
}

#include "MineInfo.h"
/// 创建地图节点
ISceneNode* CreateNode(ISceneGraph* pSceneGraph, const std::string &sInterfaceName)
{
    if(sInterfaceName == s_sPersonInfo)
    {
        return(new CPersonInfo(pSceneGraph));
    }
    else if(sInterfaceName == s_sMineInfo)
    {
        return(new CMineInfo(pSceneGraph));
    }
    else
    {
        return(nullptr);
    }
}


/// 查询接口
bool QueryInterface(std::string& sInterfaceName)
{
    sInterfaceName.clear();
    sInterfaceName += s_sPersonInfo;
    sInterfaceName += " ";
    sInterfaceName += s_sMineInfo;
    return(false);
}
