#include <sstream>
#include <QString>
#include <osgDB/ReadFile>
#include <osg/Texture2D>
#include <osg/Geometry>
#include <osg/CoordinateSystemNode>
#include <osg/LOD>
#include <osg/Callback>
#include <osgText/Text>
#include <osgUtil/CullVisitor>
#include <osg/MatrixTransform>
#include <osg/Depth>

#include <SceneGraph/ISceneGraph.h>
#include <Inner/ILoadResource.h>
#include "PlanetModel.h"

static std::map<int, const wchar_t *> g_mapPlanetName;
void InitSolarName()
{
    if(!g_mapPlanetName.empty())
    {
        return;
    }

    g_mapPlanetName[0] = L"  水星";
    g_mapPlanetName[1] = L"  金星";
    g_mapPlanetName[2] = L"  地球";
    g_mapPlanetName[3] = L"  火星";
    g_mapPlanetName[4] = L"  木星";
    g_mapPlanetName[5] = L"  土星";
    g_mapPlanetName[6] = L"  天王星";
    g_mapPlanetName[7] = L"  海王星";
    g_mapPlanetName[8] = L"  冥王星";
    g_mapPlanetName[9] = L"  月球";
    g_mapPlanetName[10] = L"  太阳";
}

/// 行星纹理名称
static const char *s_planetNames[] =
{
        "mercury",
        "venus",
        "earth",
        "mars",
        "jupiter",
        "saturn",
        "uranus",
        "neptune",
        "pluto",
        "moon"
};

const osg::Vec4 s_planetColors[] =
{
    osg::Vec4(0.90f, 0.80f, 0.80f, 1.0f),		// Mercury
    osg::Vec4(1.00f, 1.00f, 0.80f, 1.0f),		// Venus
    osg::Vec4(0.60f, 0.70f, 1.00f, 1.0f),		// Earth
    osg::Vec4(1.00f, 0.84f, 0.68f, 1.0f),		// Mars
    osg::Vec4(0.80f, 0.90f, 1.00f, 1.0f),		// Jupiter
    osg::Vec4(1.00f, 1.00f, 0.85f, 1.0f),		// Saturn
    osg::Vec4(0.75f, 0.95f, 1.00f, 1.0f),		// Uranus
    osg::Vec4(0.75f, 0.75f, 1.00f, 1.0f),		// Neptune
    osg::Vec4(1.00f, 1.00f, 1.00f, 1.0f),		// Pluto
    osg::Vec4(1.00f, 1.00f, 1.00f, 1.0f),		// Moon
};

static float s_planetRadius[][2] =
{
    {2439700, 2439700},
    {6051800, 6051800},
    {6378137, 6378137},
    {3396200, 3376200},
    {71492000, 66854000},
    {60268000, 54364000},
    {25559000, 24973000},
    {24764000, 24341000},
    {1195000, 1195000},
    {1738200, 1737400}
};

/// 构建椭球
osg::Geometry* s_makeEllipsoidGeometry(const osg::EllipsoidModel* ellipsoid,
                                       bool                       genTexCoords,
                                       int                        nSegments)
{
    osg::Geometry* geom = new osg::Geometry();
    geom->setUseVertexBufferObjects(true);

    int latSegments = nSegments;
    int lonSegments = 2 * latSegments;

    double segmentSize = 180.0/(double)latSegments; // degrees

    osg::Vec3Array* verts = new osg::Vec3Array();
    verts->reserve( (latSegments+1) * (lonSegments+1) );

    osg::Vec2Array* texCoords = nullptr;
    osg::Vec3Array* normals = nullptr;

    if (genTexCoords)
    {
        texCoords = new osg::Vec2Array();
        texCoords->reserve( (latSegments+1) * (lonSegments+1) );
        geom->setTexCoordArray( 0, texCoords );

        normals = new osg::Vec3Array(osg::Array::BIND_PER_VERTEX);
        normals->reserve( (latSegments+1) * (lonSegments+1));
        geom->setNormalArray( normals );
    }


    for( int y = 0; y <= latSegments; ++y )
    {
        osg::DrawElementsUShort* el = new osg::DrawElementsUShort(GL_TRIANGLE_STRIP);
        el->reserve( 2 * (lonSegments+1));

        double lat = -90.0 + segmentSize * (double)y;
        for( int x = 0; x <= lonSegments; ++x )
        {
            double lon = -180.0 + segmentSize * (double)x;
            double gx, gy, gz;
            ellipsoid->convertLatLongHeightToXYZ( osg::DegreesToRadians(lat), osg::DegreesToRadians(lon), 0., gx, gy, gz );
            verts->push_back( osg::Vec3(gx, gy, gz) );

            if (genTexCoords)
            {
                double s = (lon + 180) / 360.0;
                double t = (lat + 90.0) / 180.0;
                texCoords->push_back( osg::Vec2(s, t ) );
            }

            if (normals)
            {
                osg::Vec3d normal(gx, gy, gz);
                normal.normalize();
                normals->push_back( osg::Vec3f(normal) );
            }


            int y_plus_1 = y+1;
            if ( y < latSegments)
            {
                el->push_back( y*(lonSegments+1) + x);
                el->push_back( y_plus_1*(lonSegments+1) + x);
            }
        }
        geom->addPrimitiveSet(el);
    }

    geom->setVertexArray( verts );

    return geom;
}

/// 椭球包围盒
class EllipsoidBounding : public osg::Group::ComputeBoundingSphereCallback
{
private:
    /// Radius
    double _radius;
public:
    /// Constructor
    EllipsoidBounding(double radius) : _radius(radius)
    {}
    virtual osg::BoundingSphere computeBound(const osg::Node&) const
    {
        return osg::BoundingSphere(osg::Vec3(0, 0, 0), _radius);
    }
};

/// 行星节点
CPlanetModel::CPlanetModel(ISceneGraph *pSceneGraph, int nIndex)
    :m_pSceneGraph(pSceneGraph),m_nRenderBin(-1000*nIndex-100)
{
    /// 设置点的大小
    m_pUniform = new osg::Uniform("pointSize",20.0f);
    m_wsPlanetName = g_mapPlanetName[nIndex];
    m_sPlanetImage = s_planetNames[nIndex];
    m_rColor = s_planetColors[nIndex];

    m_fEquatorRadius = s_planetRadius[nIndex][0];
    m_fPolarRadius = s_planetRadius[nIndex][1];

    MakePlanet();
}

/// 行星的图标顶点着色器
osg::Shader *s_createPlanetImageVertexShader()
{
    std::stringstream buf;
    buf << "#version 330 \n";
    buf << "uniform float pointSize;\n";
    buf << "out vec4 color;\n";
    buf << "void main(void) \n"
        << "{ \n"
        << "    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; \n"
        << "    gl_PointSize = pointSize; \n"
        << "    color = gl_Color;\n"
        << "} \n";

    return new osg::Shader(osg::Shader::VERTEX, buf.str());
}

/// 行星的图标片段着色器
osg::Shader *s_createPlanetImageFragmentShader()
{
    std::stringstream buf;
    buf << "#version 330 \n";
    buf << "in vec4 color;\n";
    buf << "out vec4 fragColor;\n";
    buf << "uniform sampler2D tex0; \n";
    buf << "void main(void) \n"
        << "{ \n"
        << "    fragColor = color * texture(tex0,gl_PointCoord); \n"
        << "} \n";
    return new osg::Shader(osg::Shader::FRAGMENT, buf.str());
}

/// 行星的顶点着色器
osg::Shader *s_createPlanetVertexShader()
{
    std::stringstream buf;
    buf << "#version 330 \n";
    buf << "out vec2 coord;\n";
    buf << "void main(void) \n"
        << "{ \n"
        << "    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; \n"
        << "    coord = gl_MultiTexCoord0.xy; \n"
        << "} \n";

    return new osg::Shader(osg::Shader::VERTEX, buf.str());
}

/// 行星的片段着色器
osg::Shader *s_createPlanetFragmentShader()
{
    std::stringstream buf;
    buf << "#version 330 \n";
    buf << "in vec2 coord;\n";
    buf << "out vec4 fragColor;\n";
    buf << "uniform sampler2D tex0; \n";
    buf << "void main(void) \n"
        << "{ \n"
        << "    fragColor = texture(tex0,coord); \n"
        << "} \n";
    return new osg::Shader(osg::Shader::FRAGMENT, buf.str());
}

void CPlanetModel::MakePlanet()
{   
    m_pTransForm = new osg::Camera;
    m_pTransForm->getOrCreateStateSet()->setRenderBinDetails(m_nRenderBin, "RenderBin" );
    m_pTransForm->setRenderOrder( osg::Camera::NESTED_RENDER );
    m_pTransForm->setComputeNearFarMode( osg::CullSettings::COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES);

//    auto pAutoCamera = new osg::Camera;
//    pAutoCamera->getOrCreateStateSet()->setRenderBinDetails(m_nRenderBin+1, "RenderBin" );
//    pAutoCamera->setRenderOrder( osg::Camera::NESTED_RENDER );
//    pAutoCamera->setComputeNearFarMode( osg::CullSettings::COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES);
    addChild(m_pTransForm);
    /// 行星点目标
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;

    /// 绘制图标
    osg::ref_ptr<osg::Geometry> pImage = new osg::Geometry;
    osg::Vec3Array* pVertArray = new osg::Vec3Array;
    pVertArray->push_back(osg::Vec3(1, 0, 0));
    pImage->setVertexArray(pVertArray);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back(m_rColor);
    pImage->setColorArray(colors.get());
    pImage->setColorBinding(osg::Geometry::BIND_OVERALL);
    pImage->addPrimitiveSet(new osg::DrawArrays(GL_POINTS, 0, 1));

    /// 设置图标属性
    pImage->getOrCreateStateSet()->setTextureAttributeAndModes(0
              ,m_pSceneGraph->ResouceLoader()->LoadTexture("Data/Space/pixmaps/star.png"),osg::StateAttribute::ON);
    pImage->getOrCreateStateSet()->setMode(GL_VERTEX_PROGRAM_POINT_SIZE, osg::StateAttribute::ON);
    pImage->getOrCreateStateSet()->getOrCreateUniform("tex0",osg::Uniform::SAMPLER_2D)->set(0);
    pImage->getOrCreateStateSet()->addUniform(m_pUniform);
    osg::ref_ptr<osg::Program> pPlanetImageProgram = new osg::Program;
    pImage->getOrCreateStateSet()->setAttributeAndModes(pPlanetImageProgram.get());
    pPlanetImageProgram->addShader(s_createPlanetImageVertexShader());
    pPlanetImageProgram->addShader(s_createPlanetImageFragmentShader());

    geode->addDrawable(pImage.get());


    /// 行星名字
    m_pShowName = new osgEarth::Text;
    m_pShowName->setFontResolution(40, 40);
    m_pShowName->setColor(osg::Vec4(0, 0.25, 0.5, 1.0));
    m_pShowName->setFont(m_pSceneGraph->ResouceLoader()->LoadFont("fonts/fzsh.ttf"));

    m_pShowName->setText(m_wsPlanetName.c_str());
    m_pShowName->setCharacterSize(25);
    m_pShowName->setAxisAlignment(osgText::Text::SCREEN);
    m_pShowName->setCharacterSizeMode(osgText::Text::SCREEN_COORDS);
    m_pShowName->setBackdropType(osgText::Text::OUTLINE);
    m_pShowName->setBackdropColor(osg::Vec4(1, 1, 1, 1));
    m_pShowName->setPosition(osg::Vec3(0, 0, 0));
    m_pShowName->setAlignment(osgText::Text::LEFT_CENTER);
    geode->addDrawable(m_pShowName.get());
    osg::ref_ptr<osg::Depth> depth = new osg::Depth;
    depth->setWriteMask(false);
    getOrCreateStateSet()->setAttributeAndModes(depth.get(), osg::StateAttribute::ON);

//    m_pTransForm->addChild(geode);
    //geode->setCullingActive(false);

//    m_pTransForm->addChild(geode);
    /// 行星渲染体
    osg::ref_ptr<osg::EllipsoidModel> ellipsoid = new osg::EllipsoidModel(m_fEquatorRadius,m_fPolarRadius);

    osg::LOD* pLod = new osg::LOD;
    pLod->setRangeMode(osg::LOD::PIXEL_SIZE_ON_SCREEN);
    pLod->addChild(geode,0,100);
//    addChild(pLod);
//    pLod->setComputeBoundingSphereCallback(m_pBounding);

    osg::ref_ptr<osg::Geometry> geom = s_makeEllipsoidGeometry(ellipsoid,true,40);

    /// 行星纹理
    std::stringstream buf;
    buf << "Data/Space/pixmaps/" << m_sPlanetImage << ".jpg";
    osg::ref_ptr<osg::Texture2D> txt = m_pSceneGraph->ResouceLoader()->LoadTexture(buf.str());
    geom->getOrCreateStateSet()->setTextureAttributeAndModes(0, txt.get(),
                                                             osg::StateAttribute::ON);

    /// 设置着色器程序
    osg::ref_ptr<osg::Program> pPlanetProgram = new osg::Program;
    pPlanetProgram->addShader(s_createPlanetVertexShader());
    pPlanetProgram->addShader(s_createPlanetFragmentShader());
    geom->getOrCreateStateSet()->setAttributeAndModes(pPlanetProgram.get(),
                                                      osg::StateAttribute::ON);
    geom->getOrCreateStateSet()->getOrCreateUniform("tex0",osg::Uniform::SAMPLER_2D)->set(0);
    geode->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    geom->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

//    m_pTransForm->addChild(geom);
    pLod->addChild(geom,100,FLT_MAX);

    m_pTransForm->addChild(pLod);
}

/// 更新行星位置
void CPlanetModel::UpdatePostion(const osg::Vec3 &rPos)
{
    m_rECIPostion = rPos;
    setMatrix(osg::Matrix::translate(rPos));
}

void CPlanetModel::SetNameVisible(bool bVisble)
{
    if(bVisble)
    {
        m_pShowName->setNodeMask(-1);
    }
    else
    {
        m_pShowName->setNodeMask(0);
    }
}
