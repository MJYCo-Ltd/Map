#include <SceneGraph/ISceneGraph.h>
#include <Inner/IOsgSceneNode.h>
#include <Inner/ILoadResource.h>
#include <osgEarth/VirtualProgram>
#include <VersionMathCommon.h>
#include <osgEarth/SpatialReference>
#include "Atmosphere.h"

CAtmosphere::CAtmosphere(ISceneGraph* pSceneGraph):
    m_pSceneGraph(pSceneGraph)
{
    m_pProgramNode = new osg::Group;
}

void CAtmosphere::MakeAtmosphere()
{
    IPlot* pPlot=m_pSceneGraph->GetPlot();
    m_pEllipsoid=dynamic_cast<IEllipsoidSensor*>(pPlot->CreateSceneNode("IEllipsoidSensor"));

    m_pEllipsoid->SetDrawType(IEllipsoidSensor::FULL_PART);
    m_pEllipsoid->SetEquator(R_Earth*1.025);
    m_pEllipsoid->SetPolar(R_Earth2*1.025);
    m_pEllipsoid->ShowLine(false);
    m_pEllipsoid->SetLatSegMents(100);

    IOsgSceneNode* pOsgSceneNode = m_pEllipsoid->AsOsgSceneNode();


    m_pProgramNode->setStateSet(
                m_pSceneGraph->ResouceLoader()->GetOrCreateStateSet(ATMOSPHERE_DRAW_STATE));
    m_pProgramNode->addChild(pOsgSceneNode->GetRealNode());
}

osg::Node *CAtmosphere::GetNode()
{
    return(m_pProgramNode);
}

void CAtmosphere::SetVisible(bool bVisilbe)
{
    m_pEllipsoid->SetVisible(bVisilbe);
}
