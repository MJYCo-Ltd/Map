#include <QSet>
#include <QDebug>
#include <ISceneCore.h>
#include <Inner/ILoadResource.h>
#include <Inner/OsgExtern/OsgExtern.h>
#include "PersonInfo.h"

static QSet<IMapSceneNode*> g_allCreate;
static int g_num(0);

void my_init()
{
    ++g_num;
    qDebug()<<"my_init"<<g_num;
}

void my_fini()
{
    --g_num;
    qDebug()<<"my_fini"<<g_num;
    if(0==g_num)
    {
        for(auto one : g_allCreate)
        {
            qDebug()<<"my_fini delete";
            delete one;
        }
        g_allCreate.clear();
    }
}

#ifdef Q_OS_WIN
#include <qt_windows.h>
#undef LoadImage

BOOL WINAPI DllMain(
  _In_ HINSTANCE hinstDLL, /// 指向自身的句柄
  _In_ DWORD  fdwReason,   /// 调用原因
  _In_ LPVOID lpvReserved  /// 隐式加载和显式加载
)
{
    switch(fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        my_init();
        break;
    case DLL_PROCESS_DETACH:
        my_fini();
        break;
//    case DLL_THREAD_ATTACH:
//        my_init();
//        break;
//    case DLL_THREAD_DETACH:
//        my_fini();
//        break;
    }

    return(TRUE);
}
#else
void my_init(void) __attribute__((constructor)); //告诉gcc把这个函数扔到init section
void my_fini(void) __attribute__((destructor));  //告诉gcc把这个函数扔到fini section
#endif

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
    QtOsgEarthMapSceneNode<IPersonInfo>(pSceneGraph)
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


    m_placeStyle.getOrCreateSymbol<osgEarth::TextSymbol>()->encoding() = osgEarth::TextSymbol::ENCODING_UTF8;
    m_placeStyle.getOrCreateSymbol<osgEarth::TextSymbol>()->alignment() = osgEarth::TextSymbol::ALIGN_LEFT_CENTER;
    m_placeStyle.getOrCreateSymbol<osgEarth::TextSymbol>()->size() = 20;
    string sFontPath = GetExePath();
    sFontPath += "fonts/msyh.ttf";
    m_placeStyle.getOrCreateSymbol<osgEarth::TextSymbol>()->font() = sFontPath;
    m_placeStyle.getOrCreateSymbol<osgEarth::TextSymbol>()->fill()->color() = osgEarth::Color::White;
    m_placeStyle.getOrCreateSymbol<osgEarth::TextSymbol>()->halo()->color() = osgEarth::Color::Black;
    m_placeStyle.getOrCreate<osgEarth::AltitudeSymbol>()->clamping() =
            osgEarth::AltitudeSymbol::CLAMP_RELATIVE_TO_TERRAIN;
    m_placeStyle.getOrCreate<osgEarth::IconSymbol>()->declutter() = false;
    m_placeStyle.getOrCreate<osgEarth::IconSymbol>()->alignment() = osgEarth::IconSymbol::ALIGN_RIGHT_CENTER;

    string sImagePath = "ico/red.png";
    auto pImage = m_pSceneGraph->ResouceLoader()->LoadImage(sImagePath,32,32);


    m_pPerson = new osgEarth::PlaceNode(osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"),0,0,0),
                                        m_sName,m_placeStyle,pImage);
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
        auto pPerson = new CPersonInfo(pSceneGraph);
        g_allCreate.insert(pPerson);
        return(pPerson);
    }
    else
    {
        return(nullptr);
    }
}

/// 删除地图节点
bool DeleteNode(IMapSceneNode * pMapSceneNode)
{
    if(g_allCreate.remove(pMapSceneNode))
    {
        delete pMapSceneNode;
    }
    qDebug()<<"delete";
    return(true);
}

/// 查询接口
bool QueryInterface(string& sInterfaceName)
{
    sInterfaceName = CPersonInfo::GetInterFaceName();
    return(true);
}
