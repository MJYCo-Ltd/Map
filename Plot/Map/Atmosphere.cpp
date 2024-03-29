#include <SceneGraph/ISceneGraph.h>
#include <Inner/IOsgSceneNode.h>
#include <Inner/ILoadResource.h>
#include <osgEarth/VirtualProgram>
#include <VersionMathCommon.h>
#include <osgEarth/SpatialReference>
#include "Atmosphere.h"

CAtmosphere::CAtmosphere(ISceneGraph* pSceneGraph)
{
    IPlot* pPlot=pSceneGraph->GetPlot();
    m_pEllipsoid=dynamic_cast<IEllipsoidSensor*>(pPlot->CreateSceneNode("IEllipsoidSensor"));

    m_pEllipsoid->SetDrawType(IEllipsoidSensor::FULL_PART);
    m_pEllipsoid->SetEquator(R_Earth*1.025);
    m_pEllipsoid->SetPolar(R_Earth2*1.025);
    m_pEllipsoid->ShowLine(false);
    m_pEllipsoid->SetLatSegMents(100);

    m_pEllipsoid->MergeStateSet(ATMOSPHERE_DRAW_STATE|DONT_WRIT_DEPTH);
}

void CAtmosphere::MakeAtmosphere()
{

}

osg::Node *CAtmosphere::GetNode()
{
    return(m_pEllipsoid->AsOsgSceneNode()->GetRealNode());
}

void CAtmosphere::SetVisible(bool bVisilbe)
{
    m_pEllipsoid->SetVisible(bVisilbe);
}
