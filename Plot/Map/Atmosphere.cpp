#include <osg/PolygonMode>
#include <osg/CullFace>
#include <osg/Depth>
#include <osg/BlendFunc>
#include <osgEarth/GLUtils>
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
    auto atmosSet = pOsgNode->getOrCreateStateSet();
    atmosSet->setAttributeAndModes(
                new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL),
                osg::StateAttribute::PROTECTED);

    osgEarth::GLUtils::setLighting(atmosSet, osg::StateAttribute::OFF);
    atmosSet->setAttributeAndModes( new osg::CullFace(osg::CullFace::FRONT), osg::StateAttribute::ON );
    atmosSet->setAttributeAndModes( new osg::Depth( osg::Depth::LESS, 0, 1, false ) ); // no depth write
    atmosSet->setAttributeAndModes( new osg::Depth(osg::Depth::ALWAYS, 0, 1, false) ); // no zbuffer
    atmosSet->setAttributeAndModes( new osg::BlendFunc( GL_ONE, GL_ONE ), osg::StateAttribute::ON );

    osgEarth::VirtualProgram* vp = osgEarth::VirtualProgram::getOrCreate( atmosSet );
    vp->setName("SimpleSkyAtmosphere");
    m_pSceneGraph->ResouceLoader()->LoadVirtualProgram(vp,"GLSL/Atmosphere.glsl");
    vp->setInheritShaders(false);
}

osg::Node *CAtmosphere::GetNode()
{
    return(m_pEllipsoid->AsOsgSceneNode()->GetOsgNode());
}

void CAtmosphere::SetVisible(bool bVisilbe)
{
    m_pEllipsoid->SetVisible(bVisilbe);
}