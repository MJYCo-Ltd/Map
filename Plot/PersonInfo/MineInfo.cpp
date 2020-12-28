#include <ISceneCore.h>
#include "MineInfo.h"

void CMineInfo::UpdateMapNode(osgEarth::MapNode *pMapNode)
{
    m_pMine->setMapNode(pMapNode);
    if(pMapNode->isGeocentric())
    {
        m_pMine->setHorizonCulling(true);
    }
}

void CMineInfo::InitNode()
{
    ImplMapSceneNode<IMineInfo>::InitNode();


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

    std::string sImagePath = "ico/17.png";
    auto pImage = m_pSceneGraph->ResouceLoader()->LoadImage(sImagePath,32,32);


    m_pMine = new osgEarth::PlaceNode(osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"),0,0,0),
                                      m_sName,m_placeStyle,pImage);
    m_pMine->setDynamic(true);
    ImplMapSceneNode<IMineInfo>::SetOsgNode(m_pMine.get());
}

void CMineInfo::PosChanged()
{
    m_bPosChanged = true;
    ImplMapSceneNode<IMineInfo>::NodeChanged();
}

void CMineInfo::NameChanged()
{
    m_bNameChanged = true;
    ImplMapSceneNode<IMineInfo>::NodeChanged();
}

///更新节点
void CMineInfo::UpdateNode()
{
    if(m_bPosChanged)
    {
        m_pMine->setPosition(osgEarth::GeoPoint(
                                   osgEarth::SpatialReference::get("wgs84"),
                                   m_geoPos.fLon,m_geoPos.fLat,m_geoPos.fHeight));
        m_bPosChanged=false;
    }

    if(m_bNameChanged)
    {
        m_pMine->setText(m_sName);
        m_bNameChanged=false;
    }

    ImplMapSceneNode<IMineInfo>::UpdateNode();
}
