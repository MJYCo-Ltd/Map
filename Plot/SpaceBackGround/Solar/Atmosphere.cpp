#include <osg/PolygonMode>
#include <osg/CullFace>
#include <osg/Depth>
#include <osg/BlendFunc>
#include <osgEarth/GLUtils>
#include <SceneGraph/ISceneGraph.h>
#include <Inner/IOsgSceneNode.h>
#include <Inner/ILoadResource.h>
#include <osgEarth/VirtualProgram>
#include "Atmosphere.h"

CAtmosphere::CAtmosphere(ISceneGraph* pSceneGraph,int nIndex):
    m_pSceneGraph(pSceneGraph),
    m_nIndex(nIndex)
{
}

void CAtmosphere::MakeAtmosphere()
{
    IPlot* pPlot=m_pSceneGraph->GetPlot();
    m_pCameraNode=pPlot->CreateSceneGroup(CAMERA_GROUP)->AsSceneCameraGroup();
    m_pEllipsoid=dynamic_cast<IEllipsoidSensor*>(pPlot->CreateSceneNode("IEllipsoidSensor"));

    m_pCameraNode->AddSceneNode(m_pEllipsoid);

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
    vp->setName( "SimpleSky Atmosphere");
    vp->setInheritShaders( false );
    m_pSceneGraph->ResouceLoader()->LoadVirtualProgram(vp,"GLSL/Atmosphere.glsl");
}