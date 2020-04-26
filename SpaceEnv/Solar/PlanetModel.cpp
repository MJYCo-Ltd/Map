#include <sstream>
#include <QString>
#include <QDebug>

#include <osgDB/ReadFile>
#include <osg/Texture2D>
#include <osg/Geometry>
#include <osg/CoordinateSystemNode>
#include <osg/LOD>
#include <osg/Callback>
#include <osg/PointSprite>
#include <osgText/Text>
#include <osgUtil/CullVisitor>

#include <SceneGraph/ISceneGraph.h>
#include <Inner/ILoadResource.h>
#include "PlanetModel.h"


/// 行星回调更新
class PlanetUpdateCallback:public osg::NodeCallback
{
public:

    PlanetUpdateCallback(CPlanetModel* pParent):m_pParent(pParent){}

    /// 回调
    void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        /// 如果是更新回调
        if(osg::NodeVisitor::UPDATE_VISITOR == nv->getVisitorType())
        {
            if(m_pParent->m_bNeedUpdate)
            {
                m_pParent->setMatrix(osg::Matrix::translate(m_pParent->m_rECIPostion));
                m_pParent->m_bNeedUpdate = false;
            }
        }/// 如果是裁剪回调
        else if(osg::NodeVisitor::CULL_VISITOR == nv->getVisitorType())
        {
            osgUtil::CullVisitor *cv = static_cast<osgUtil::CullVisitor*>(nv);
            /// 更新在眼睛坐标系下的坐标
            m_pParent->m_rEyePostion = m_pParent->m_rECIPostion - nv->getEyePoint();

            /// 重新计算像素大小
            float fPixelSize = cv->pixelSize(m_pParent->m_rECIPostion,m_pParent->m_fEquatorRadius);


            if (fPixelSize <= 20)
            {
                if (fPixelSize >= 0.5)
                {
                    fPixelSize = 15 + 5 * sinf(osg::PI_2 * fPixelSize * 0.05123);
                }
                else if (fPixelSize >= 0.1)
                {
                    fPixelSize = 10 + 5 * sinf(osg::PI_2 * fPixelSize * 2.5);
                }
                else
                {
                    fPixelSize = 5 + 5 * sinf(osg::PI_2 * fPixelSize * 10.0);
                }
            }
            else
            {
                fPixelSize = 20;
            }

            fPixelSize *= 1.5f;

            /// 更新统一变量
            m_pParent->m_pUniform->set(fabs(fPixelSize));

        }

        /// 遍历所有节点
        traverse(node,nv);
    }
private:
    CPlanetModel* m_pParent;   /// 根节点
};

/// 行星点节点绘制回调
class PlantDrawCallback : public osg::Drawable::DrawCallback
{
public:
    PlantDrawCallback(CPlanetModel *parent):m_pParent(parent){}

    /// 绘制回调
    virtual void drawImplementation(
            osg::RenderInfo& rRenderInfo, const osg::Drawable* pDrawable) const
    {
        /// 如果被裁剪则不渲染
        //if(!m_pParent->m_bCulled) return;

        /// 获取投影矩阵
        osg::ref_ptr<osg::RefMatrix> oldProjectionMatrix = new osg::RefMatrix;
        oldProjectionMatrix->set(rRenderInfo.getState()->getProjectionMatrix());

        /// 获取视角，纵横比，远近裁剪面
        double fovy, aspectRatio, zNear, zFar;
        oldProjectionMatrix->getPerspective(fovy, aspectRatio, zNear, zFar);

        double dEyeDistance = m_pParent->m_rEyePostion.length();

        bool bNeedReset=false;
        if(dEyeDistance < zNear)
        {
            dEyeDistance -= m_pParent->m_fEquatorRadius;

            osg::ref_ptr<osg::RefMatrixd> projectionMatrix = new osg::RefMatrix;
            projectionMatrix->makePerspective(fovy, aspectRatio,dEyeDistance*0.9,dEyeDistance*9000);
            rRenderInfo.getState()->applyProjectionMatrix(projectionMatrix);

            bNeedReset = true;
        }
        else if(dEyeDistance > zFar)
        {
            dEyeDistance += m_pParent->m_fEquatorRadius;
            osg::ref_ptr<osg::RefMatrixd> projectionMatrix = new osg::RefMatrix;
            projectionMatrix->makePerspective(fovy, aspectRatio,dEyeDistance*0.000101,dEyeDistance*1.01);
            rRenderInfo.getState()->applyProjectionMatrix(projectionMatrix);

            bNeedReset = true;
        }

        if(bNeedReset)
        {
            rRenderInfo.getState()->applyModelViewAndProjectionUniformsIfRequired();
        }

        /// 绘制函数
        pDrawable->drawImplementation(rRenderInfo);

        if(bNeedReset)
        {
            rRenderInfo.getState()->applyProjectionMatrix(oldProjectionMatrix);
            rRenderInfo.getState()->applyModelViewAndProjectionUniformsIfRequired();
        }
    }

private:
    CPlanetModel *m_pParent;  /// 行星节点
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
    verts->reserve( latSegments * lonSegments );

    osg::Vec2Array* texCoords = nullptr;
    osg::Vec3Array* normals = nullptr;

    if (genTexCoords)
    {
        texCoords = new osg::Vec2Array();
        texCoords->reserve( latSegments * lonSegments );
        geom->setTexCoordArray( 0, texCoords );

        normals = new osg::Vec3Array(osg::Array::BIND_PER_VERTEX);
        normals->reserve( latSegments * lonSegments );
        geom->setNormalArray( normals );
    }

    osg::DrawElementsUShort* el = new osg::DrawElementsUShort( GL_TRIANGLES );
    el->reserve( latSegments * lonSegments * 6 );

    for( int y = 0; y <= latSegments; ++y )
    {
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


            if ( y < latSegments )
            {
                int x_plus_1 = x < lonSegments ? x+1 : 0;
                int y_plus_1 = y+1;
                el->push_back( y*lonSegments + x + y);
                el->push_back( y*lonSegments + x_plus_1 + y);
                el->push_back( y_plus_1*lonSegments + x + y_plus_1);

                el->push_back( y*lonSegments + x_plus_1 + y);
                el->push_back( y_plus_1*lonSegments + x_plus_1 + y_plus_1);
                el->push_back( y_plus_1*lonSegments + x + y_plus_1);
            }
        }
    }

    geom->setVertexArray( verts );
    geom->addPrimitiveSet( el );

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
CPlanetModel::CPlanetModel(ISceneGraph *pSceneGraph)
    :m_pSceneGraph(pSceneGraph)
    ,m_bNeedUpdate(false)
    ,m_bCulled(true)
{
    /// 添加更新回调
    m_pCallBack = new PlanetUpdateCallback(this);
    m_pDrawBack = new PlantDrawCallback(this);

    /// 设置点的大小
    m_pUniform = new osg::Uniform("pointSize",20.0f);

    /// 增加回调
    this->addUpdateCallback(m_pCallBack);
    this->addCullCallback(m_pCallBack);
}

/// 设置行星名称
void CPlanetModel::SetName(const wchar_t *pPlanetName, const char *pImageName)
{
    m_wsPlanetName = pPlanetName;
    m_sPlanetImage = pImageName;
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
    pImage->setDrawCallback(m_pDrawBack);

    /// 设置图标属性
    pImage->getOrCreateStateSet()->setTextureAttributeAndModes(0
              ,m_pSceneGraph->ResouceLoader()->LoadTexture("SpaceResource/pixmaps/star.png"),osg::StateAttribute::ON);
    osg::PointSprite *sprite = new osg::PointSprite();
    pImage->getOrCreateStateSet()->setTextureAttributeAndModes(0, sprite, osg::StateAttribute::ON);
    pImage->getOrCreateStateSet()->setMode(GL_VERTEX_PROGRAM_POINT_SIZE, osg::StateAttribute::ON);
    pImage->getOrCreateStateSet()->getOrCreateUniform("tex0",osg::Uniform::SAMPLER_2D)->set(0);
    pImage->getOrCreateStateSet()->addUniform(m_pUniform);
    osg::ref_ptr<osg::Program> pPlanetImageProgram = new osg::Program;
    pImage->getOrCreateStateSet()->setAttributeAndModes(pPlanetImageProgram.get());
    pPlanetImageProgram->addShader(s_createPlanetImageVertexShader());
    pPlanetImageProgram->addShader(s_createPlanetImageFragmentShader());

    geode->addDrawable(pImage.get());


    /// 行星名字
    m_pShowName = new osgText::Text;
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
    m_pShowName->setDrawCallback(m_pDrawBack);
    geode->addDrawable(m_pShowName.get());

    this->addChild(geode);
    geode->setCullingActive(false);

    /// 行星渲染体
    osg::ref_ptr<osg::EllipsoidModel> ellipsoid = new osg::EllipsoidModel(m_fEquatorRadius,m_fPolarRadius);

    osg::LOD* pLod = new osg::LOD;
    pLod->setRangeMode(osg::LOD::PIXEL_SIZE_ON_SCREEN);
    pLod->setComputeBoundingSphereCallback(m_pBounding);

    osg::ref_ptr<osg::Geometry> geom = s_makeEllipsoidGeometry(ellipsoid,true,40);

    /// 行星纹理
    std::stringstream buf;
    buf << "SpaceResource/pixmaps/" << m_sPlanetImage << ".png";
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
    geom->setDrawCallback(m_pDrawBack);

    pLod->addChild(geom.get(), 15, FLT_MAX);

    this->addChild(pLod);
}

/// 更新行星位置
void CPlanetModel::UpdatePostion(const osg::Vec3 &rPos)
{
    m_rECIPostion = rPos;
    m_bNeedUpdate = true;
}

/// 更新行星视点位置
void CPlanetModel::UpdateEyePostion(const osg::Vec3 &rEyePos)
{
    m_rEyePostion = rEyePos;
}

/// 设置颜色
void CPlanetModel::SetColor(const osg::Vec4 &rColor)
{
    m_rColor = rColor;
}

/// 设置半径
void CPlanetModel::SetRadius(float fEquatorRadius, float fPolarRadius)
{
    m_fEquatorRadius = fEquatorRadius;
    m_fPolarRadius = fPolarRadius;
    m_pBounding = new EllipsoidBounding(fEquatorRadius);
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
