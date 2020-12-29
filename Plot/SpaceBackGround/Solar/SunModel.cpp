#include <osg/Billboard>
#include <osg/Geometry>
#include <osg/BlendFunc>
#include <osg/Depth>
#include <osg/FrontFace>
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
            double x = cos(angle);
            double y = sin(angle);
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
        state->setAttributeAndModes(new osg::BlendFunc(
                                        osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA),
                                    osg::StateAttribute::ON);
        state->setAttributeAndModes(new osg::FrontFace(
                                        osg::FrontFace::CLOCKWISE), osg::StateAttribute::ON);

        osg::ref_ptr<osg::Depth> depth = new osg::Depth;
        depth->setWriteMask(false);
        state->setAttributeAndModes(depth.get(), osg::StateAttribute::ON);
    }
    /// Implementation of the draw function
    virtual void drawImplementation(osg::RenderInfo &renderInfo) const
    {
        osg::Camera *camera = renderInfo.getCurrentCamera();
        if (!camera)
        {
            osg::Geometry::drawImplementation(renderInfo);
            return;
        }

        double s = SUN_RADIUS;

        // Get the current camera position.
        osg::Vec3 eye, center, up;
        camera->getViewMatrixAsLookAt(eye, center, up);

        // Save old values.
        osg::ref_ptr<osg::RefMatrix> oldProjectionMatrix = new osg::RefMatrix;
        oldProjectionMatrix->set(renderInfo.getState()->getProjectionMatrix());

        // Get the individual values
        double left, right, bottom, top, zNear, zFar;
        oldProjectionMatrix->getFrustum(left, right, bottom, top, zNear, zFar);

        // Save the model view matrix
        static osg::Matrix oldModelView;
        oldModelView.set(renderInfo.getState()->getModelViewMatrix());

        // Get the max distance we need the far plane to be at,
        // which is the distance between the eye and the origin
        // plus the distant from the origin to the object (star sphere
        // radius, sun distance etc), and then some.
        double distance = eye.length() + s * 2.0;

        // Build a new projection matrix with a modified far plane
        osg::ref_ptr<osg::RefMatrix> projectionMatrix = new osg::RefMatrix;

        if (distance > zFar * 0.8)
        {
            double zNearNew = fmax(distance * 5e-3, 1);
            //offset = (zFar * 0.99 - distance);
            double aspect = zNearNew / zNear;
            projectionMatrix->makeFrustum(
                        left * aspect, right * aspect, bottom * aspect, top * aspect,
                        zNearNew, distance * 1.25);
            renderInfo.getState()->applyProjectionMatrix(projectionMatrix.get());
        }

        // Scale the model
        static osg::Matrix mat;
        mat.set(oldModelView);
        mat.preMult(osg::Matrix::scale(s, s, s));
        renderInfo.getState()->applyModelViewMatrix(mat);

        renderInfo.getState()->applyModelViewAndProjectionUniformsIfRequired();
        // Draw the drawable
        osg::Geometry::drawImplementation(renderInfo);

        // Reset the far plane to the old value.
        renderInfo.getState()->applyProjectionMatrix(oldProjectionMatrix.get());
        renderInfo.getState()->applyModelViewMatrix(oldModelView);
        renderInfo.getState()->applyModelViewAndProjectionUniformsIfRequired();
    }
};

class SunUpdateCallback:public osg::Callback
{
public:

    SunUpdateCallback(CSunModel* pParent):m_pParent(pParent){}

    /// 回调
    bool run(osg::Object* object, osg::Object* data)
    {
        if(m_pParent->m_bNeedUpdate)
        {
            m_pParent->setMatrix(osg::Matrix::translate(m_pParent->m_rECIPostion));
            m_pParent->m_bNeedUpdate = false;
        }
        return traverse(object, data);
    }
private:
    CSunModel* m_pParent;
};

/// 太阳模型
CSunModel::CSunModel():m_bNeedUpdate(false)
{
    this->addUpdateCallback(new SunUpdateCallback(this));

    osg::ref_ptr<osg::Billboard> bd = new osg::Billboard;
    bd->setMode(osg::Billboard::POINT_ROT_EYE);
    bd->addDrawable(new SunGeometry);
    addChild(bd.get());
}

/// 更新位置
void CSunModel::UpdatePostion(const osg::Vec3 &rPos)
{
    m_rECIPostion = rPos;
    m_bNeedUpdate = true;
}
