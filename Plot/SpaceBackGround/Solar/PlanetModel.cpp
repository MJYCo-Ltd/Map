#include <sstream>
#include <QString>
#include <osgDB/ReadFile>
#include <osg/Texture2D>
#include <osg/Geometry>
#include <osg/CoordinateSystemNode>
#include <osg/LOD>
#include <osg/Callback>
#include <osgText/Text>
#include <osgUtil/CullVisitor>
#include <osg/MatrixTransform>
#include <osg/Depth>

#include <SceneGraph/ISceneGraph.h>
#include <Inner/ILoadResource.h>
#include <Inner/IOsgSceneNode.h>
#include "PlanetModel.h"

static std::map<int, const char*> g_mapPlanetName;
void InitSolarName()
{
    if(!g_mapPlanetName.empty())
    {
        return;
    }

    g_mapPlanetName[0] = "水星";
    g_mapPlanetName[1] = "金星";
    g_mapPlanetName[2] = "地球";
    g_mapPlanetName[3] = "火星";
    g_mapPlanetName[4] = "木星";
    g_mapPlanetName[5] = "土星";
    g_mapPlanetName[6] = "天王星";
    g_mapPlanetName[7] = "海王星";
    g_mapPlanetName[8] = "冥王星";
    g_mapPlanetName[9] = "月球";
    g_mapPlanetName[10] = "太阳";
}

/// 行星纹理名称
static const char *s_planetNames[] =
{
        "mercury",
        "venus",
        "earth",
        "mars",
        "jupiter",
        "saturn",
        "uranus",
        "neptune",
        "pluto",
        "moon"
};

const osg::Vec4 s_planetColors[] =
{
    osg::Vec4(0.90f, 0.80f, 0.80f, 1.0f),		// Mercury
    osg::Vec4(1.00f, 1.00f, 0.80f, 1.0f),		// Venus
    osg::Vec4(0.60f, 0.70f, 1.00f, 1.0f),		// Earth
    osg::Vec4(1.00f, 0.84f, 0.68f, 1.0f),		// Mars
    osg::Vec4(0.80f, 0.90f, 1.00f, 1.0f),		// Jupiter
    osg::Vec4(1.00f, 1.00f, 0.85f, 1.0f),		// Saturn
    osg::Vec4(0.75f, 0.95f, 1.00f, 1.0f),		// Uranus
    osg::Vec4(0.75f, 0.75f, 1.00f, 1.0f),		// Neptune
    osg::Vec4(1.00f, 1.00f, 1.00f, 1.0f),		// Pluto
    osg::Vec4(1.00f, 1.00f, 1.00f, 1.0f),		// Moon
};

static float s_planetRadius[][2] =
{
    {2439700, 2439700},
    {6051800, 6051800},
    {6378137, 6378137},
    {3396200, 3376200},
    {71492000, 66854000},
    {60268000, 54364000},
    {25559000, 24973000},
    {24764000, 24341000},
    {1195000, 1195000},
    {1738200, 1737400}
};

/// 行星节点
CPlanetModel::CPlanetModel(ISceneGraph *pSceneGraph, int nIndex)
    :m_pSceneGraph(pSceneGraph),m_nRenderBin(-1000*nIndex-100)
{
    /// 设置点的大小
    m_cnPlanetName = g_mapPlanetName[nIndex];
    m_sPlanetImage = s_planetNames[nIndex];
    m_rColor = s_planetColors[nIndex];

    m_fEquatorRadius = s_planetRadius[nIndex][0];
    m_fPolarRadius = s_planetRadius[nIndex][1];

    MakePlanet();
}

CPlanetModel::~CPlanetModel()
{
}

void CPlanetModel::MakePlanet()
{
    IPlot* pPlot=m_pSceneGraph->GetPlot();
    m_pAttitudeNode = pPlot->CreateSceneGroup(ATTITUDE_GROUP)->AsSceneAttitudeGroup();
    m_pLodNode=pPlot->CreateSceneGroup(LOD_GROUP)->AsSceneLodGroup();
    m_pCameraNode=pPlot->CreateSceneGroup(CAMERA_GROUP)->AsSceneCameraGroup();
    m_pPointNode=dynamic_cast<IPoint*>(pPlot->CreateSceneNode("IPoint"));
    m_pEllipsoid=dynamic_cast<IEllipsoidSensor*>(pPlot->CreateSceneNode("IEllipsoidSensor"));

    m_pAttitudeNode->AddSceneNode(m_pCameraNode);
    m_pCameraNode->AddSceneNode(m_pLodNode);
    m_pLodNode->AddSceneNode(m_pEllipsoid);
    m_pLodNode->AddSceneNode(m_pPointNode);
    m_pLodNode->SetNodeChangeType(m_pLodNode->NODE_SCREEN_PIXEL);

    m_pCameraNode->SetRenderIndex(m_nRenderBin);
    /// 设置显示层级
    std::vector<float> vLevelInfo;
    vLevelInfo.push_back(20.f);
    m_pLodNode->SetLevelsInfo(vLevelInfo);

    SceneColor sceneColor;
    sceneColor.fR = m_rColor.r();
    sceneColor.fG = m_rColor.g();
    sceneColor.fB = m_rColor.b();
    sceneColor.fA = m_rColor.a();

    m_pPointNode->SetPointSize(20.f);
    m_pPointNode->SetColor(sceneColor);
    m_pPointNode->SetImage("Space/pixmaps/star.png");

    m_pEllipsoid->SetShowBack(false);
    m_pEllipsoid->SetDrawType(m_pEllipsoid->FULL_PART);
    m_pEllipsoid->SetEquator(m_fEquatorRadius);
    m_pEllipsoid->SetPolar(m_fPolarRadius);
    m_pEllipsoid->ShowLine(false);
    std::string sPath = "Space/pixmaps/";
    sPath += m_sPlanetImage;
    sPath += ".png";
    m_pEllipsoid->SetImage(sPath);
}

/// 更新行星位置
void CPlanetModel::UpdatePostion(const Math::CVector &rPos)
{
    ScenePos pos;
    pos.fX = rPos.GetX();
    pos.fY = rPos.GetY();
    pos.fZ = rPos.GetZ();
    m_pAttitudeNode->SetPos(pos);
}

osg::Node *CPlanetModel::GetNode()
{
    return(m_pAttitudeNode->AsOsgSceneNode()->GetOsgNode());
}
