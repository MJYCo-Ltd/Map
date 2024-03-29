#include <osg/Billboard>
#include <osg/Geometry>
#include <osg/BlendFunc>
#include <osg/Depth>
#include <osg/FrontFace>
#include <osg/MatrixTransform>
#include "SunModel.h"

std::string s_getSunVertexSource()
{
    static std::string sunVertex(
                "#version 330 \n"
                "out vec3 atmos_v3Direction; \n"
                "void main() \n"
                "{ \n"
                "    vec3 v3Pos = gl_Vertex.xyz; \n"
                "    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; \n"
                "    atmos_v3Direction = vec3(0.0,1.0,0.0) - v3Pos; \n"
                "    atmos_v3Direction = atmos_v3Direction/length(atmos_v3Direction); \n"
                "} \n");
    return(sunVertex);
}

std::string s_getSunFragmentSource()
{
    static std::string sunFragment(
                "#version 330 \n"

                "float fastpow( in float x, in float y ) \n"
                "{ \n"
                "    return x/(x+y-y*x); \n"
                "} \n"

                "in vec3 atmos_v3Direction; \n"
                "out vec4 fragColor;\n"
                "void main( void ) \n"
                "{ \n"
                "   float fCos = -atmos_v3Direction[1]; \n"
                "   float fMiePhase = 0.050387596899224826 * (1.0 + fCos*fCos) / fastpow(1.9024999999999999 - -1.8999999999999999*fCos, 1.5); \n"
                "   fragColor.rgb = fMiePhase*vec3(.3,.3,.2); \n"
                "   fragColor.a = (fragColor.r - 0.2) * 1.25; \n"
                "} \n");

    return (sunFragment);
}

#define SUN_RADIUS (6.960e8 * 20)

class SunGeometry: public osg::Geometry
{
public:
    SunGeometry()
    {
//        this->setDrawCallback(new MyDrawCallBack("Sun"));
        setCullingActive(false);
        int segments = 50;
        float deltaAngle = osg::PI * 2.0 / float(segments);

        osg::Vec3Array *verts = new osg::Vec3Array();
        verts->reserve(2 + segments);
        setVertexArray(verts);

        addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLE_FAN,0,2 + segments));

        verts->push_back(osg::Vec3(0, 0, 0));

        for (int i = 0; i <= segments; i ++)
        {
            double angle = deltaAngle * float(i);
            double x = 10*SUN_RADIUS * cos(angle);
            double y = 10*SUN_RADIUS * sin(angle);
            verts->push_back(osg::Vec3(x, 0, y));
        }

        osg::StateSet *state = getOrCreateStateSet();

        // create shaders
        osg::Program* program = new osg::Program();
        osg::Shader* vs = new osg::Shader(
                    osg::Shader::VERTEX, s_getSunVertexSource());
        program->addShader(vs);
        osg::Shader* fs = new osg::Shader(
                    osg::Shader::FRAGMENT, s_getSunFragmentSource());
        program->addShader(fs);
        state->setAttributeAndModes(program, osg::StateAttribute::ON);

        // configure the stateset
        state->setTextureMode(0, GL_TEXTURE_2D, osg::StateAttribute::OFF);
        state->setMode(GL_BLEND,osg::StateAttribute::ON);
        state->setAttributeAndModes(new osg::FrontFace(
                                        osg::FrontFace::CLOCKWISE), osg::StateAttribute::ON);

        osg::ref_ptr<osg::Depth> depth = new osg::Depth;
        depth->setWriteMask(false);
        state->setAttribute(depth.get());
    }
};

/// 太阳模型
CSunModel::CSunModel():m_bNeedUpdate(false)
{

    m_pTrans = new osg::Camera;
    m_pTrans->getOrCreateStateSet()->setRenderBinDetails( -100001, "RenderBin" );
    m_pTrans->setRenderOrder(osg::Camera::NESTED_RENDER);
    m_pTrans->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES);

    osg::ref_ptr<osg::Billboard> bd = new osg::Billboard;
    bd->setMode(osg::Billboard::POINT_ROT_EYE);
    bd->addDrawable(new SunGeometry);
    m_pTrans->addChild(bd);
    addChild(m_pTrans);
}

/// 更新位置
void CSunModel::UpdatePostion(const Math::CVector &rPos)
{
    setMatrix(osg::Matrix::translate(osg::Vec3(rPos.GetX(),rPos.GetY(),rPos.GetZ())));
}
