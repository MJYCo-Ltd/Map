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

    float _innerRadius=osg::minimum(R_Earth2,R_Earth);
    // calculate and apply the uniforms:
    // TODO: perhaps we can just hard-code most of these as GLSL consts.
    float r_wl = ::powf( .65f, 4.0f );
    float g_wl = ::powf( .57f, 4.0f );
    float b_wl = ::powf( .475f, 4.0f );
    osg::Vec3 RGB_wl(1.0f/r_wl, 1.0f/g_wl, 1.0f/b_wl);
    float Kr = 0.0025f;
    float Kr4PI = Kr * 4.0f * osg::PI;
    float Km = 0.0015f;
    float Km4PI = Km * 4.0f * osg::PI;
    float ESun = 15.0f;
    float MPhase = -.095f;
    float RayleighScaleDepth = 0.25f;
    int   Samples = 2;
    float Weather = 1.0f;

    float Scale = 1.0f / (.025f * _innerRadius);

    //TODO: make all these constants. -gw
    atmosSet->getOrCreateUniform( "atmos_v3InvWavelength", osg::Uniform::FLOAT_VEC3 )->set( RGB_wl);
    atmosSet->getOrCreateUniform( "atmos_fInnerRadius",    osg::Uniform::FLOAT )->set(_innerRadius);
    atmosSet->getOrCreateUniform( "atmos_fInnerRadius2",   osg::Uniform::FLOAT )->set(_innerRadius * _innerRadius);
    atmosSet->getOrCreateUniform( "atmos_fKrESun",         osg::Uniform::FLOAT )->set(Kr * ESun);
    atmosSet->getOrCreateUniform( "atmos_fKmESun",         osg::Uniform::FLOAT )->set(Km * ESun);
    atmosSet->getOrCreateUniform( "atmos_fKr4PI",          osg::Uniform::FLOAT )->set(Kr4PI);
    atmosSet->getOrCreateUniform( "atmos_fKm4PI",          osg::Uniform::FLOAT )->set(Km4PI);
    atmosSet->getOrCreateUniform( "atmos_fScale",          osg::Uniform::FLOAT )->set(Scale);
    atmosSet->getOrCreateUniform( "atmos_fScaleDepth",     osg::Uniform::FLOAT )->set(RayleighScaleDepth);
    atmosSet->getOrCreateUniform( "atmos_fScaleOverScaleDepth", osg::Uniform::FLOAT )->set(Scale/RayleighScaleDepth);
    atmosSet->getOrCreateUniform( "atmos_g",               osg::Uniform::FLOAT )->set(MPhase);
    atmosSet->getOrCreateUniform( "atmos_g2",              osg::Uniform::FLOAT )->set(MPhase * MPhase);
    atmosSet->getOrCreateUniform( "atmos_nSamples",        osg::Uniform::INT )->set(Samples);
    atmosSet->getOrCreateUniform( "atmos_fSamples",        osg::Uniform::FLOAT )->set((float)Samples);
    atmosSet->getOrCreateUniform( "atmos_fWeather",        osg::Uniform::FLOAT )->set(Weather);

    auto pStateSet = m_pSceneGraph->ResouceLoader()->LoadVirtualProgram("GLSL/Atmosphere.glsl");
    osgEarth::VirtualProgram* vp = dynamic_cast<osgEarth::VirtualProgram*>(pStateSet->getAttribute(osg::StateAttribute::PROGRAM));
    vp->setInheritShaders(false);
    pOsgNode->setStateSet(m_pSceneGraph->ResouceLoader()->MergeStateSet(pStateSet,atmosSet));
}

osg::Node *CAtmosphere::GetNode()
{
    return(m_pEllipsoid->AsOsgSceneNode()->GetOsgNode());
}

void CAtmosphere::SetVisible(bool bVisilbe)
{
    m_pEllipsoid->SetVisible(bVisilbe);
}
