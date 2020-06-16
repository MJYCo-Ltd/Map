#include <ISceneCore.h>
#include <Inner/ILoadResource.h>
#include <Inner/OsgExtern/OsgExtern.h>
#include "PersonInfo.h"

string CPersonInfo::S_sInterFace("IPersonInfo");

class PersonInfoCallBack:public osg::Callback
{
public:
    PersonInfoCallBack(osgEarth::PlaceNode* pPlaceNode):m_pPlaceNode(pPlaceNode)
    {
    }

    bool run(osg::Object* object, osg::Object* data)
    {
        if(m_bUpdatePos)
        {
            m_pPlaceNode->setPosition(osgEarth::GeoPoint(
                                          osgEarth::SpatialReference::get("wgs84"),
                                          m_stPos.fLon,m_stPos.fLat,m_stPos.fHeight));
            m_bUpdatePos = false;
        }

        if(m_bUpdateName)
        {
            m_pPlaceNode->setText(m_sName);
            m_bUpdateName = false;
        }
        return traverse(object, data);
    }

    /// 更新位置
    void SetPotsion(const ScenePos & rScenPos)
    {
        m_stPos = rScenPos;
        m_bUpdatePos = true;
    }

    /// 更新名字
    void UpdateName(const string& sName)
    {
        m_sName = sName;
        m_bUpdateName = true;
    }
protected:
    virtual ~PersonInfoCallBack() {}
private:
    ScenePos m_stPos;
    string   m_sName;
    bool     m_bUpdatePos  = false;
    bool     m_bUpdateName = false;
    osg::observer_ptr<osgEarth::PlaceNode> m_pPlaceNode;
};

CPersonInfo::CPersonInfo(ISceneGraph*pSceneGraph):
    QtOsgSceneNode<IPersonInfo>(pSceneGraph)
{
}

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
void CPersonInfo::InitSceneNode()
{
    QtOsgSceneNode<IPersonInfo>::InitSceneNode();


    m_placeStyle.getOrCreateSymbol<osgEarth::TextSymbol>()->encoding() = osgEarth::Symbology::TextSymbol::ENCODING_UTF8;
    m_placeStyle.getOrCreateSymbol<osgEarth::TextSymbol>()->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_LEFT_CENTER;
    m_placeStyle.getOrCreateSymbol<osgEarth::TextSymbol>()->size() = 20;
    string sFontPath = GetExePath();
    sFontPath += "fonts/msyh.ttf";
    m_placeStyle.getOrCreateSymbol<osgEarth::TextSymbol>()->font() = sFontPath;
    m_placeStyle.getOrCreateSymbol<osgEarth::TextSymbol>()->fill()->color() = osgEarth::Color::White;
    m_placeStyle.getOrCreateSymbol<osgEarth::TextSymbol>()->halo()->color() = osgEarth::Color::Black;
    m_placeStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() =
            osgEarth::Symbology::AltitudeSymbol::CLAMP_RELATIVE_TO_TERRAIN;
    m_placeStyle.getOrCreate<osgEarth::IconSymbol>()->declutter() = false;
    m_placeStyle.getOrCreate<osgEarth::IconSymbol>()->alignment() = osgEarth::IconSymbol::ALIGN_RIGHT_CENTER;

    string sImagePath = "ico/red.png";
    auto pImage = m_pSceneGraph->ResouceLoader()->LoadImage(sImagePath,32,32);


    m_pPerson = new osgEarth::PlaceNode(osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"),0,0,0),
                                        "test",m_placeStyle,pImage);
    m_pPerson->setDynamic(true);
    m_pCallBack = new PersonInfoCallBack(m_pPerson);
    m_pPerson->addUpdateCallback(m_pCallBack);
    m_pOsgNode->addChild(m_pPerson.get());

}

/// 位置更新消息
void CPersonInfo::PosChanged()
{
    m_pCallBack->SetPotsion(m_unScenePos);
}

/// 设置名字
void CPersonInfo::SetName(const string &strName)
{
    if(m_sName != strName)
    {
        m_sName = strName;
        m_pCallBack->UpdateName(m_sName);
    }
}

const string &CPersonInfo::GetName()
{
    return(m_sName);
}

/// 创建地图节点
IPersonInfo* CreateNode(ISceneGraph* pSceneGraph, const string &sInterfaceName)
{
    if(sInterfaceName == CPersonInfo::GetInterFaceName())
    {
        return(new CPersonInfo(pSceneGraph));
    }
    else
    {
        return(nullptr);
    }
}

/// 查询接口
bool QueryInterface(string& sInterfaceName)
{
    sInterfaceName = CPersonInfo::GetInterFaceName();
    return(false);
}
