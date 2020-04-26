#include <QString>
#include <osg/CoordinateSystemNode>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osg/TexEnv>
#include <osg/FrontFace>
#include <osg/Geode>
#include <osg/Program>
#include <osgEarth/VirtualProgram>

#include <SceneGraph/ISceneGraph.h>
#include <Inner/ILoadResource.h>

#include "Milkyway.h"

static const char* vert =
        "#version " GLSL_VERSION_STR "\n"
        "out vec2 mikyCoords;\n"
        "void myVertShader(inout vec4 vertex) { \n"
        "    mikyCoords = gl_MultiTexCoord0.xy; \n"
        "} \n";

static const char* frag =
        "#version " GLSL_VERSION_STR "\n"
        "in vec2 mikyCoords;\n"
        "uniform sampler2D mikyWay;\n"
        "out vec4 out_Color; \n \n"
        "void myFragShader(inout vec4 color) { \n"
        "    out_Color = color * texture(mikyWay,mikyCoords); \n"
        "} \n";

extern osg::Geometry* s_makeEllipsoidGeometry(const osg::EllipsoidModel* ellipsoid,
                                              bool                       genTexCoords,
                                              int                        nSegments);

CMilkyway::CMilkyway(ISceneGraph *pSceneGraph):
    m_pSceneGraph(pSceneGraph)
{
    makeMilkyway();
}

osg::Node *CMilkyway::getNode()
{
    return(m_pMilkyway.get());
}

CMilkyway::~CMilkyway()
{
}

/// 构建银河
void CMilkyway::makeMilkyway()
{
//    osg::Matrix rot = osg::Matrix::rotate(
//                        osg::DegreesToRadians(7.0), osg::Vec3(0, 0, 1),
//                        osg::DegreesToRadians(120.0), osg::Vec3(0, 1, 0),
//                        osg::DegreesToRadians(23.0), osg::Vec3(1, 0, 0));
//    m_pTransform = new osg::MatrixTransform;
//    m_pTransform->setMatrix(rot);
    osg::TessellationHints* pHints = new osg::TessellationHints;

    /// 纹理坐标
    pHints->setCreateTextureCoords(true);
    pHints->setCreateFrontFace(false);
    pHints->setCreateBackFace(true);
    pHints->setCreateNormals(false);
    pHints->setCreateTop(false);
    pHints->setCreateBody(false);
    pHints->setCreateBottom(false);

    osg::Sphere* pShape = new osg::Sphere(osg::Vec3(),1.f);
    m_pMilkyway = new osg::ShapeDrawable(pShape,pHints);

    m_pMilkyway->setColor(osg::Vec4f(1.0,1.0,1.0,1.0));

    osg::StateSet *state = m_pMilkyway->getOrCreateStateSet();

    /// 添加纹理
    osg::Texture2D *txt = m_pSceneGraph->ResouceLoader()->LoadTexture("SpaceResource/pixmaps/milkyway.png");
    txt->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
    txt->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
    state->setAttributeAndModes(
            new osg::FrontFace(osg::FrontFace::CLOCKWISE),
            osg::StateAttribute::ON|osg::StateAttribute::PROTECTED);
    state->setTextureAttributeAndModes(0, txt);
    state->getOrCreateUniform("mikyWay",osg::Uniform::SAMPLER_2D)->set(0);


    osgEarth::VirtualProgram* vp = osgEarth::VirtualProgram::getOrCreate(state);
    vp->setFunction("myVertShader",vert,osgEarth::ShaderComp::LOCATION_VERTEX_MODEL);
    vp->setFunction( "myFragShader", frag, osgEarth::ShaderComp::LOCATION_FRAGMENT_OUTPUT);
}
