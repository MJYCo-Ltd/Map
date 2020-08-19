#include <osg/NodeCallback>
#include <osg/BlendFunc>
#include <osgUtil/CullVisitor>

#include <ISceneCore.h>
#include <SceneGraph/ISceneGraph.h>

#include "Satellite/JPLEphemeris.h"
#include "Math/Vector.h"
#include "Satellite/Date.h"
#include "Satellite/TimeSys.h"

#include "SolarEnv.h"


static std::map<int, const wchar_t *> g_mapPlanetName;
void InitSolarName()
{
    if(!g_mapPlanetName.empty())
    {
        return;
    }

    g_mapPlanetName[0] = L"  水星";
    g_mapPlanetName[1] = L"  金星";
    g_mapPlanetName[2] = L"  地球";
    g_mapPlanetName[3] = L"  火星";
    g_mapPlanetName[4] = L"  木星";
    g_mapPlanetName[5] = L"  土星";
    g_mapPlanetName[6] = L"  天王星";
    g_mapPlanetName[7] = L"  海王星";
    g_mapPlanetName[8] = L"  冥王星";
    g_mapPlanetName[9] = L"  月球";
    g_mapPlanetName[10] = L"  太阳";
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

/// 太阳系构建函数
CSolarEnv::CSolarEnv(ISceneGraph *pSceneGraph)
    :m_pSceneGraph(pSceneGraph)
    ,m_bNeedUpdate(false)
{
    /// 加载JPL星历
    if(!Aerospace::CJPLEphemeris::GetInstance()->IsInit())
    {
        Aerospace::CJPLEphemeris::GetInstance()->Init(GetExePath()+"SpaceResource/dynamics/ephem");
    }

    InitSolarName();

    osg::StateSet *state = getOrCreateStateSet();


    state->setMode(GL_BLEND, osg::StateAttribute::ON);

    state->setAttributeAndModes(
                new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA,
                                   osg::BlendFunc::ONE_MINUS_SRC_ALPHA),
                osg::StateAttribute::ON);
}

/// 构建太阳系
void CSolarEnv::CreateSolar()
{

    for (int i=0;i<10; ++i)
    {
        /// 跳过地球
        if(2 == i) continue;

        CPlanetModel* pPlanet = new CPlanetModel(m_pSceneGraph);
        pPlanet->SetName(g_mapPlanetName[i],s_planetNames[i]);
        pPlanet->SetColor(s_planetColors[i]);
        pPlanet->SetRadius(s_planetRadius[i][0],s_planetRadius[i][1]);

        m_mapPlanet[i] = pPlanet;

        pPlanet->MakePlanet();
        this->addChild(pPlanet);
    }

    m_pSun = new CSunModel;
    this->addChild(m_pSun);
}

/// 更新时间
void CSolarEnv::UpdateTime(const double &dMJD)
{
    Aerospace::CTimeSys timeSys(dMJD);
    double dMJDTT = timeSys.GetTT();

    for(std::map<int,osg::ref_ptr<CPlanetModel> >::iterator itor = m_mapPlanet.begin();
        itor != m_mapPlanet.end(); ++itor)
    {
        Math::CVector tmpPos = Aerospace::CJPLEphemeris::GetInstance()
                ->GetPos(dMJDTT,PLANET_TYPE(itor->first+1),Earth);
        if(tmpPos)
        {
            itor->second->UpdatePostion(osg::Vec3(tmpPos.GetX(),tmpPos.GetY(),tmpPos.GetZ()));
        }
    }

    Math::CVector tmpPos = Aerospace::CJPLEphemeris::GetInstance()
            ->GetPos(dMJDTT,Sun,Earth);
    if(tmpPos)
    {
        m_pSun->UpdatePostion(osg::Vec3(tmpPos.GetX(),tmpPos.GetY(),tmpPos.GetZ()));
    }
}

void CSolarEnv::traverse(osg::NodeVisitor &nv)
{
    if(osg::NodeVisitor::CULL_VISITOR ==nv.getVisitorType())
    {
        osgUtil::CullVisitor* cv = nv.asCullVisitor();

        /// 如果是裁剪遍历
        if (cv)
        {
            osg::CullSettings::ComputeNearFarMode saveMode = cv->getComputeNearFarMode();
            cv->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);

            osg::MatrixTransform::traverse(nv);
            cv->setComputeNearFarMode(saveMode);

            return;
        }
    }

    osg::MatrixTransform::traverse(nv);
}

void CSolarEnv::SetPlanetNameShow(bool bVisible)
{
    for(auto one : m_mapPlanet)
    {
        one.second->SetNameVisible(bVisible);
    }
}
