#include <osg/NodeCallback>
#include <osg/BlendFunc>
#include <osgUtil/CullVisitor>

#include <ISceneCore.h>
#include <SceneGraph/ISceneGraph.h>

#include <Math/VecMat.h>
#include <Satellite/TimeSys.h>
#include <Satellite/JPLEphemeris.h>

#include "SolarEnv.h"

extern void InitSolarName();
/// 太阳系构建函数
CSolarEnv::CSolarEnv(ISceneGraph *pSceneGraph)
    :m_pSceneGraph(pSceneGraph)
    ,m_bNeedUpdate(false)
{
    InitSolarName();

    osg::StateSet *state = getOrCreateStateSet();

    state->setMode(GL_BLEND,osg::StateAttribute::ON);
}

CSolarEnv::~CSolarEnv()
{
    for(auto itor = m_mapPlanet.begin();
        itor != m_mapPlanet.end();
        ++itor)
    {
        delete itor->second;
    }
    m_mapPlanet.clear();
}

/// 构建太阳系
void CSolarEnv::CreateSolar()
{

    for (int i=0;i<10; ++i)
    {
        /// 跳过地球
        if(2 == i) continue;

        CPlanetModel* pPlanet = new CPlanetModel(m_pSceneGraph,i);

        m_mapPlanet[i] = pPlanet;
        this->addChild(pPlanet->GetNode());
    }

    m_pSun = new CSunModel;
    this->addChild(m_pSun);
}

void CSolarEnv::UpdatePos(const std::vector<Math::CVector> &vSolarPos)
{
    for(auto itor = m_mapPlanet.begin();
        itor != m_mapPlanet.end();
        ++itor)
    {
        itor->second->UpdatePostion(vSolarPos[itor->first]);
    }

    m_pSun->UpdatePostion(vSolarPos[10]);
}

void CSolarEnv::SetPlanetNameShow(bool bVisible)
{
}
