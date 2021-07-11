#include <cmath>
#include <VersionMathCommon.h>
#include <Inner/OsgExtern/MathCommon.h>
#include <Inner/OsgExtern/IOsgMapSceneNode.h>
bool IOsgMapSceneNode::s_bIs3D{false};
osg::observer_ptr<osgEarth::MapNode>                IOsgMapSceneNode::s_pMapNode;
osg::observer_ptr<const osgEarth::SpatialReference> IOsgMapSceneNode::s_pMapSRS;
osg::observer_ptr<osgEarth::Terrain>          IOsgMapSceneNode::s_pTerrain;
osg::ref_ptr<osgEarth::SpatialReference> IOsgMapSceneNode::s_pWGS84=osgEarth::SpatialReference::get("WGS84");

CMathCommon *CMathCommon::GetInstance()
{
    static CMathCommon s_mathCommon;

    /// 如果没有初始化 则初始化
    if(!s_mathCommon.m_bInit)
    {
        s_mathCommon.Init();
    }

    return(&s_mathCommon);
}

/// 获取sin值
const double *CMathCommon::GetSin()
{
    return(m_pSin);
}

/// 获取cos值
const double* CMathCommon::GetCos()
{
    return(m_pCos);
}

CMathCommon::CMathCommon():
    m_bInit(false)
{
    m_pSin = new double[360];
    m_pCos = new double[360];
}

/// 析构数据空间
CMathCommon::~CMathCommon()
{
    delete []m_pSin;
    delete []m_pCos;
}

/// 初始化
void CMathCommon::Init()
{
    for(int i=0; i<360; ++i)
    {
        m_pSin[i] = sin(i*DD2R);
        m_pCos[i] = cos(i*DD2R);
    }
    m_bInit = true;
}
