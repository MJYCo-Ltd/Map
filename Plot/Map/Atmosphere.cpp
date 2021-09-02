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
}
#include <iostream>
void CAtmosphere::MakeAtmosphere()
{
    IPlot* pPlot=m_pSceneGraph->GetPlot();
    m_pEllipsoid=dynamic_cast<IEllipsoidSensor*>(pPlot->CreateSceneNode("IEllipsoidSensor"));

    m_pEllipsoid->SetDrawType(IEllipsoidSensor::FULL_PART);
    m_pEllipsoid->SetEquator(R_Earth2*1.025);
    m_pEllipsoid->SetPolar(R_Earth*1.025);
    m_pEllipsoid->ShowLine(false);
    m_pEllipsoid->SetLatSegMents(100);

    IOsgSceneNode* pOsgSceneNode = m_pEllipsoid->AsOsgSceneNode();
    auto pOsgNode = pOsgSceneNode->GetOsgNode();

    auto pStateSet = m_pSceneGraph->ResouceLoader()->LoadVirtualProgram("GLSL/Atmosphere.glsl");

    pOsgNode->setStateSet(pStateSet);
}

osg::Node *CAtmosphere::GetNode()
{
    return(m_pEllipsoid->AsOsgSceneNode()->GetOsgNode());
}

void CAtmosphere::SetVisible(bool bVisilbe)
{
    m_pEllipsoid->SetVisible(bVisilbe);
}
