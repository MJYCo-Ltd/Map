#include <QString>
#include <QDateTime>

#include <osg/Vec3>
#include <osgDB/ReadFile>
#include <osg/Texture2D>
#include <osg/Image>
#include <osg/Uniform>
#include <osg/PointSprite>
#include <osg/Point>
#include <osg/BlendFunc>

#include <SceneGraph/ISceneGraph.h>
#include <Inner/ILoadResource.h>
#include "StarRender.h"
#include "GeodesicGrid.h"



#if (!defined(OSG_GLES2_AVAILABLE) && !defined(OSG_GLES3_AVAILABLE))
#define GLSL_VERSION_STR "330 core"
#else
#define GLSL_VERSION_STR "300 es"
#endif

static const float EYE_RESOLUTION (0.25f);
static const float MAX_LINEAR_RADIUS (8.f);

std::string s_getStarVertexSource()
{
    return std::string(
                "#version " GLSL_VERSION_STR "\n"
                "out vec4 color; \n"
                "out float magnitude; \n"
                "out float pointSize; \n"
                "void main(void) \n"
                "{  \n"
                "   magnitude = gl_Color.a * 1e3; \n"
                "   color = gl_Color; \n"
                "	color.a = 1; \n"
                "   if (magnitude < 6.0) \n"
                "       gl_PointSize = 12.0 - magnitude * 2.0;  \n"
                "   else  \n"
                "       gl_PointSize = 1.0;  \n"
                "	 gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; \n"
                "    pointSize = gl_PointSize; \n"
                "} \n");
}

std::string s_getStarFragmentSource()
{
    return std::string(
                "#version " GLSL_VERSION_STR "\n"
                "in vec4 color;\n"
                "in float magnitude; \n"
                "in float pointSize; \n"
                "out vec4 fragColor; \n"
                "uniform sampler2D star0; \n"
                "uniform sampler2D star1; \n"
                "uniform float maxMag; \n"
                "void main(void) \n"
                "{ \n"
                "  if ((maxMag > 0.0) && (magnitude > maxMag)) \n"
                "    discard; \n"
                "  if (pointSize > 2.0) \n"
                "	   fragColor = color * texture2D(star0, gl_PointCoord); \n"
                "  else \n"
                "    fragColor = color *  texture2D(star1, gl_PointCoord); \n"
                "  fragColor.a = 1; \n"
                "} \n");
}

CStarRender::CStarRender(ISceneGraph *pSceneGraph, float fShowMaxMag)
    :m_pSceneGraph(pSceneGraph)
    ,m_fShowMaxMag(fShowMaxMag)
    ,m_pGeodesicGrid(nullptr)
{
    m_StarTR.setWorldAdaptationLuminance(1.0);

    init();
}

/// 渲染回调
int CStarRender::renderFunc(int level, unsigned int index, const osg::Vec3 &c0,
                            const osg::Vec3 &c1, const osg::Vec3 &c2, void *context, int value)
{
    StarsRenderContext *sctx = (StarsRenderContext *) context;
    osg::Camera *camera = sctx->pOsgRenderInfo->getCurrentCamera();
    if (!camera)
        return Frustum::Out;

    if (value == Frustum::PartIn)
    {
        static osg::Vec3 unit_c0, unit_c1, unit_c2;
        unit_c0.set(c0);
        unit_c0.normalize();
        unit_c1.set(c1);
        unit_c1.normalize();
        unit_c2.set(c2);
        unit_c2.normalize();
        value = sctx->pBuilder->m_Frustum.checkFrustum(unit_c0, unit_c1,
                                                       unit_c2);
    }

    if (value > 0 && level >= 0)
    {
        for (size_t i = 0; i < sctx->pBuilder->m_vRenderVector.size(); i++)
        {
            if (sctx->pBuilder->m_vRenderVector[i]->getLevel() == level)
            {
                osg::Geometry *geom =
                        sctx->pBuilder->m_vRenderVector[i]->getGeometry(sctx->pBuilder->m_fShowMaxMag, index);
                if (geom)
                {
                    geom->draw(*sctx->pOsgRenderInfo);
                }

                break;
            }
        }
    }

    return value;
}


/// 设置网格
void CStarRender::setGeodesicGrid(ZoneArrayVector &vZoneData, GeodesicGrid *pGeodesicGrid
                                  , int nMaxLevel, const QMap<int, QString> &mapStarName, osg::Group *pRoot, double dJD)
{
    m_vRenderVector.clear();

    m_pGeodesicGrid = pGeodesicGrid;
    m_nMaxLevel = nMaxLevel;

    /// 清空所有的渲染结构体
    m_vRenderVector.clear();

    for(int i=0; i<vZoneData.size(); ++i)
    {
        osg::ref_ptr<CStarZoneArrayRenderer> render = new CStarZoneArrayRenderer(m_pSceneGraph,
                    vZoneData[i], dJD,m_pGeodesicGrid,mapStarName);


        const std::vector<osg::ref_ptr<osgText::Text> >& crStarNames = render->getStarNames();
        for(std::vector<osg::ref_ptr<osgText::Text> >::const_iterator itorName = crStarNames.begin();crStarNames.end() != itorName; ++itorName)
        {
            pRoot->addChild(*itorName);
        }
        m_vRenderVector.push_back(render);
    }
}

/// 重写绘制
void CStarRender::drawImplementation(osg::RenderInfo &renderInfo) const
{
    double fov, zNear, zFar, aspect;
    if (!renderInfo.getState()->getModelViewMatrix().getPerspective(fov,
                                                                    aspect, zNear, zFar))
    {
        fov = 45.0;
    }

    CStarRender* This = (CStarRender*)this;
    This->updateEye(fov);
    This->setupState();

    StarsRenderContext ctx;
    ctx.pBuilder = This;
    ctx.pOsgRenderInfo = &renderInfo;
    This->m_Frustum.update(renderInfo.getState()->getModelViewMatrix(),
                           renderInfo.getState()->getProjectionMatrix());

    /// 渲染星星
    m_pGeodesicGrid->visitTriangles(m_nMaxLevel, renderFunc, &ctx);
}

CStarRender::~CStarRender()
{
}

void CStarRender::init()
{
    osg::StateSet *state = getOrCreateStateSet();


    osg::ref_ptr<osg::Texture2D> starTexture1 = m_pSceneGraph->ResouceLoader()->LoadTexture("SpaceResource/pixmaps/asterism.png");
    state->setTextureAttributeAndModes(0, starTexture1.get());

    osg::ref_ptr<osg::Texture2D> starTexture2 = m_pSceneGraph->ResouceLoader()->LoadTexture("SpaceResource/pixmaps/star.png");
    state->setTextureAttributeAndModes(1, starTexture2.get());

    osg::ref_ptr<osg::PointSprite> sprite = new osg::PointSprite();
    state->setTextureAttributeAndModes(0, sprite);
    state->setTextureAttributeAndModes(1, sprite);

    osg::ref_ptr<osg::Program> starsProgram = new osg::Program();
    osg::ref_ptr<osg::Shader> vs = new osg::Shader(osg::Shader::VERTEX,
                                      s_getStarVertexSource());
    starsProgram->addShader(vs);
    osg::ref_ptr<osg::Shader> fs = new osg::Shader(osg::Shader::FRAGMENT,
                                      s_getStarFragmentSource());
    starsProgram->addShader(fs);
    state->setAttributeAndModes(starsProgram.get());

    state->getOrCreateUniform("star0", osg::Uniform::SAMPLER_2D)->set(0);
    state->getOrCreateUniform("star1", osg::Uniform::SAMPLER_2D)->set(1);

    /// 设置点大小
    state->setMode(GL_PROGRAM_POINT_SIZE,osg::StateAttribute::ON);
    state->setMode(GL_POINT_SMOOTH,osg::StateAttribute::ON);
}

void CStarRender::setupState()
{
    osg::StateSet *state = getOrCreateStateSet();

    state->getOrCreateUniform("maxMag", osg::Uniform::FLOAT)->set(m_fShowMaxMag);
}

void CStarRender::updateEye(float fov)
{
    if (fov < 45)
        fov = 45;
    if (fov > 60)
        fov = 60;
}
