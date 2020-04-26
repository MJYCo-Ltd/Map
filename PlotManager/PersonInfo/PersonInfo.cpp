#include <QDebug>
#include <ISceneCore.h>
#include <Inner/ILoadResource.h>
#include <Inner/OsgExtern/OsgExtern.h>
#include "PersonInfo.h"

int nTestCount(0);
class Test:public osgEarth::PlaceNode
{
public:
    Test(const osgEarth::GeoPoint&    position,
              const std::string& text  ="",
              const osgEarth::Style&       style =osgEarth::Style(),
              osg::Image*        image =0L):osgEarth::PlaceNode(position,text,style,image){}
    ~Test()
    {
        int i=0;
        --i;
        qDebug()<<"PlaceNode "<<++nTestCount;
    }
};

CPersonInfo::CPersonInfo(ISceneGraph*pSceneGraph):
    QtSceneNode<IPersonInfo>(pSceneGraph)
{
}

CPersonInfo::~CPersonInfo()
{
}

void CPersonInfo::UpdateMapNode(osgEarth::MapNode *pMapNode)
{
    return;
    m_pPerson->setMapNode(pMapNode);
    if(pMapNode->isGeocentric())
    {
        m_pPerson->setHorizonCulling(true);
    }
}


void CPersonInfo::InitSceneNode()
{
    QtSceneNode<IPersonInfo>::InitSceneNode();


    m_placeStyle.getOrCreateSymbol<osgEarth::TextSymbol>()->encoding() = osgEarth::Symbology::TextSymbol::ENCODING_UTF8;
    m_placeStyle.getOrCreateSymbol<osgEarth::TextSymbol>()->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_CENTER;
    string sFontPath = GetExePath();
    sFontPath += "font/msyh.ttf";
    m_placeStyle.getOrCreateSymbol<osgEarth::TextSymbol>()->font() = sFontPath;
    m_placeStyle.getOrCreateSymbol<osgEarth::TextSymbol>()->fill()->color() = osgEarth::Color::White;
    m_placeStyle.getOrCreateSymbol<osgEarth::TextSymbol>()->halo()->color() = osgEarth::Color::Black;
    m_placeStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() =
            osgEarth::Symbology::AltitudeSymbol::CLAMP_RELATIVE_TO_TERRAIN;
    m_placeStyle.getOrCreate<osgEarth::IconSymbol>()->declutter() = false;
    m_placeStyle.getOrCreate<osgEarth::IconSymbol>()->alignment() = osgEarth::IconSymbol::ALIGN_RIGHT_CENTER;

    string sImagePath = "ico/red.png";
    auto pImage = m_pSceneGraph->ResouceLoader()->LoadImage(sImagePath,32,32);

    m_pPerson = new Test(osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"),120.,24.,100),
                                        "test",m_placeStyle,pImage);
    m_pOsgNode->addChild(m_pPerson.get());
}

void CPersonInfo::SetPos(const ScenePos &pScenePos)
{
    m_pPerson->setPosition(osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"),pScenePos.fLon,pScenePos.fLat,100));
}

void CPersonInfo::Add()
{
    string sImagePath = "ico/red.png";

    for(int i=0;i<10000; ++i)
    {
//        auto pImage = m_pSceneGraph->ResouceLoader()->LoadImage(sImagePath,32,32);
        osg::ref_ptr<osgEarth::PlaceNode> pLocal = new Test(osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"),120+i*0.1,24+i*0.1,100),
                                            "",m_placeStyle,nullptr);
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CModifyNode(m_pOsgNode,pLocal,true));
    }
}

void CPersonInfo::Del()
{
    m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CClearChildNode(m_pOsgNode));
}

IPersonInfo* CreateNode(ISceneGraph* pSceneGraph)
{
    return(new CPersonInfo(pSceneGraph));
}
