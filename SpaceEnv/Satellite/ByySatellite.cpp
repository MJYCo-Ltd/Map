#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <osg/NodeCallback>
#include <osg/ShadeModel>
#include <osg/LineWidth>
#include <osg/BlendFunc>
#include <osgEarth/PhongLightingEffect>
#include <QApplication>
#include <Intpol.h>
#include "ByySatellite.h"
#include "../SnModelLoad.h"
#include "../DrawShape/ByyRenderCone.h"
#include "../DrawShape/SnSatelliteUnderCover.h"
#include "../DrawShape/SnDraw2DSatelliteOribit.h"
#include "ByyTransform.h"
#include "../DrawShape/SnRenderCone.h"

static const int SIZE(3);
/// 卫星位置回调函数
class SatelliteUpdateCallBack:public osg::NodeCallback
{
public:
    SatelliteUpdateCallBack(CByySatellite* pSatellite,osg::MatrixTransform* pNode):
        m_pSatellite(pSatellite),m_pSatelliteNode(pNode) {}

    /// 更新
    void operator ()(osg::Node *node, osg::NodeVisitor *nv)
    {
        if(node == m_pSatelliteNode && m_pSatellite->m_bUpdateSatellite)
        {
            static Math::CVector rZ,rY,rX;
            Math::CVector pos = m_pSatellite->m_stNowPos.slice(0,2);
            Math::CVector vPos = m_pSatellite->m_stNowPos.slice(3,5);

            rZ = -pos;
            rY = vPos;

            rY = Math::CVecMat::Cross(rZ , rY);
            rX = Math::CVecMat::Cross(rY,rZ);

            rX.Normalize();
            rY.Normalize();
            rZ.Normalize();

            /// 设置矩阵
            m_pSatelliteNode->setMatrix(osg::Matrix(
                                            rX.GetX(),rX.GetY(),rX.GetZ(),0,
                                            rY.GetX(),rY.GetY(),rY.GetZ(),0,
                                            rZ.GetX(),rZ.GetY(),rZ.GetZ(),0,
                                            pos.GetX(),pos.GetY(),pos.GetZ(), 1
                                            ));

            m_pSatellite->m_bUpdateSatellite = false;
        }
        traverse(node,nv);
    }

    /**
     * @brief 获取矩阵
     * @return
     */
    Math::CMatrix& Matrix(){return(m_matTransla);}
private:
    CByySatellite*               m_pSatellite;   /// 卫星管理类
    osg::MatrixTransform* m_pSatelliteNode;/// 卫星位置类
    Math::CMatrix             m_matTransla;     /// 平移矩阵
};

/// 卫星轨道更新
class OribitUpdateCallBack : public osg::NodeCallback
{
public:
    OribitUpdateCallBack(CByySatellite* pSatellite,osg::Geometry* pGeometry)
        :m_pSatellite(pSatellite),m_pNode(pGeometry){}

    /// 更新
    void operator ()(osg::Node *node, osg::NodeVisitor *nv)
    {
        if(node == m_pNode && m_pSatellite->m_bUpdateOribit)
        {
            osg::ref_ptr<osg::Vec3Array> pArray = new osg::Vec3Array;

            for(unsigned int i=0; i<m_pSatellite->m_vOribit.size(); ++i)
            {
                pArray->push_back(osg::Vec3(m_pSatellite->m_vOribit[i].GetX(),
                                            m_pSatellite->m_vOribit[i].GetY(),
                                            m_pSatellite->m_vOribit[i].GetZ()));
            }
            osg::ref_ptr<osg::Vec4Array> pColor = new osg::Vec4Array;
            pColor->push_back(osg::Vec4(21./255.,148./255.,203./255.,1.f));

            /// 绑定顶点数组
            m_pNode->setVertexArray(pArray);

            /// 绑定颜色数组
            m_pNode->setColorArray(pColor,osg::Array::BIND_PER_PRIMITIVE_SET);

            /// 移除所有的
            m_pNode->removePrimitiveSet(0,m_pNode->getNumPrimitiveSets());
            m_pNode->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP,0,pArray->size()));

            m_pSatellite->m_bUpdateOribit = false;
        }

        traverse(node,nv);
    }

private:
    CByySatellite* m_pSatellite; /// 数据指针
    osg::Geometry* m_pNode; /// node指针
};

#include <QDebug>
/// 卫星类
CByySatellite::CByySatellite(osg::Group *pRoot):
    m_dStart(0.0),
    m_dEnd(0.0),
    m_dStep(0.0),
    m_bUpdateSatellite(false),
    m_bUpdateOribit(false),
    m_bCreateUnder(false),
    m_nIndex(-1),
    m_pRoot(pRoot),
    m_emMapType(MAP_3D),
    m_dYaw(0),
    m_dPitch(0),
    m_dRoll(0),
    m_sZiTai("YPR")
{

        QString sAppPath = QApplication::applicationDirPath();

        sAppPath += "/data/model/ice/ice.3ds";
//        sAppPath += "/data/model/ets8.obj";
        m_pSatelliteNode = CSnModelLoad::GetInstance()->LoadModel(sAppPath.toStdString());

        m_pSatelliteNode->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::ON);
        m_pSatelliteNode->setNodeMask(0x0000ff00);

        osg::MatrixTransform* pTransform =  new osg::MatrixTransform;
        pTransform->addChild(m_pSatelliteNode);
        pTransform->setMatrix(osg::Matrix::scale(5000.,5000.,5000.));
        m_pSatellite = pTransform;

    /// 初始化
    Init();
}

/// 析构函数释放信息
CByySatellite::~CByySatellite()
{
    delete m_p2DOribit;
    delete m_pCone;
    delete m_pUnderCover;
}

/// 设置模型名称
void CByySatellite::SetModelName(const std::string &sModelName)
{
     osg::Node* pNode = CSnModelLoad::GetInstance()->LoadModel(sModelName);

     if(pNode)
     {
         pNode->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::ON);
         pNode->setNodeMask(0x0000ff00);
         osg::MatrixTransform* pTranform = static_cast<osg::MatrixTransform*>(m_pSatellite.get());
         if(pTranform)
         {
             pTranform->removeChildren(0,pTranform->getNumChildren());
             m_pSatelliteNode = pNode;
             pTranform->addChild(m_pSatelliteNode);
             pTranform->dirtyBound();
         }
     }
}

/// 设置卫星位置
void CByySatellite::SetOribitData(const std::vector<CVector> &rOribit)
{
    /// 复制数据
    m_vOribit = rOribit;

    m_bUpdateOribit = true;
}

/// 设置卫星的经纬信息
void CByySatellite::SetGeoOribit(const std::vector<CVector> &vOribit)
{
    /// 设置轨道
    m_p2DOribit->SetOribitInfo(vOribit);
}


std::string s_OribitgetMapVertexSource()
{
    return std::string(
            "out vec4 vertex_color; \n"
            "void main()													\n"
            "{																		\n"
            "  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;	\n"
            "  vertex_color = gl_Color; \n"
            "}																		\n"
            );
}

std::string s_OribitgetMapFragmentSource()
{
    return std::string(
            "in vec4 point_color; \n"
            "void main()																\n"
            "{																					\n"
            "   gl_FragColor = point_color; \n"
            "}																					\n"
            );
}

std::string s_OribitGetMapGeometrySource()
{
    return std::string(
                "#version 330 \n"
                "layout (lines) in; \n"
                "layout (triangle_strip,max_vertices = 6) out; \n"
                " in vec4 vertex_color[]; \n"
                " out vec4 point_color; \n"
                "void main() \n"
                "{ \n"
                "   vec2 ab = gl_in[1].gl_Position.xy - gl_in[0].gl_Position.xy; \n"
                "   float abLength = length(ab); \n"
                "   float fLength = abLength * 0.02; \n"
                "   vec2 tmp2d ;  \n"
                "   tmp2d.x = ab.x;  \n"
                "   tmp2d.y = 0 - ab.y;  \n"
                "   vec2 ac = normalize(tmp2d); \n"
                "   // 1 \n"
                "   gl_Position = gl_in[0].gl_Position; \n"
                "   gl_Position.xy = gl_in[0].gl_Position.xy - ac * fLength; \n"
                "   point_color = vertex_color[0]; \n"
                "   point_color.a = 0; \n"
                "   EmitVertex(); \n "
                "   // 2 \n"
                "   gl_Position = gl_in[1].gl_Position; \n"
                "   gl_Position.xy = gl_in[1].gl_Position.xy - ac * fLength; \n"
                "   point_color = vertex_color[1]; \n"
                "   point_color.a = 0; \n"
                "   EmitVertex(); \n "
                "   // 3 \n"
                "   gl_Position = gl_in[0].gl_Position; \n"
                "   point_color = vertex_color[0]; \n"
                "   EmitVertex(); \n "
                "   // 4 \n"
                "   gl_Position = gl_in[1].gl_Position; \n"
                "   point_color = vertex_color[1]; \n"
                "   EmitVertex(); \n "
                "   // 5 \n"
                "   gl_Position = gl_in[0].gl_Position; \n"
                "   gl_Position.xy = gl_in[0].gl_Position.xy + ac * fLength; \n"
                "   point_color = vertex_color[0]; \n"
                "   point_color.a = 0; \n"
                "   EmitVertex(); \n "
                "   // 6 \n"
                "   gl_Position = gl_in[1].gl_Position; \n"
                "   gl_Position.xy = gl_in[1].gl_Position.xy + ac * fLength; \n"
                "   point_color = vertex_color[1]; \n"
                "   point_color.a = 0; \n"
                "   EmitVertex(); \n "
                " EndPrimitive(); \n"
                "} \n"
                );
}

#include <QApplication>
/// 初始化参数
void CByySatellite::Init()
{
    m_stNowPos.Resize(6);

    /// 构建轨道显示
    m_pOribit = new osg::Geometry();
    m_pOribit->setUpdateCallback(new OribitUpdateCallBack(this,m_pOribit));


    /// 构建卫星显示类
    m_pSatellitePos = new osg::MatrixTransform;
    m_pSatellitePos->addChild(m_pSatellite);
    m_pSatellitePos->setUpdateCallback(new SatelliteUpdateCallBack(this,m_pSatellitePos));

    /// 添加到group节点上
    this->addChild(m_pOribit);
    this->addChild(m_pSatellitePos);

    m_p2DOribit = new CSnDraw2DSatelliteOribit;
    m_pCone = new CByyRenderCone;
    m_pUnderCover = new CSnSatelliteUnderCover;
    m_pCone->SetDistance(100.);
    m_pConeZiTai = new osg::MatrixTransform;
    m_pConeZiTai->addChild(m_pCone->CreateNode());
    m_pSatellitePos->addChild(m_pConeZiTai.get());

    osg::Geode* _textGeode = new osg::Geode;
    _textGeode->setCullingActive(false);
    osg::StateSet *state = _textGeode->getOrCreateStateSet();
    state->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    state->setTextureMode(0, GL_TEXTURE_2D, osg::StateAttribute::ON);
    state->setAttributeAndModes(
            new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA,
                    osg::BlendFunc::ONE_MINUS_SRC_ALPHA),
            osg::StateAttribute::ON);
    text= new osgText::Text;
    text->setFontResolution(40, 40);

//    text->setDrawMode(3);
    text->setFont(QString("%1/data/font/msyh.ttf").arg(QApplication::applicationDirPath()).toStdString());
    text->setText(QString::fromLocal8Bit("hello").toUtf8().data(),osgText::String::ENCODING_UTF8);
    text->setColor(osg::Vec4(0,1.0,0.0,0.8));
    text->setCharacterSize(20);
    text->setAxisAlignment(osgText::Text::SCREEN);
    text->setCharacterSizeMode(osgText::Text::SCREEN_COORDS);
    text->setAutoRotateToScreen(true);
//    text->setBackdropType(osgText::Text::OUTLINE);
//    text->setBackdropColor(osg::Vec4(1, 1, 0., 0.5));
    text->setPosition(osg::Vec3(0, 0, 0));
    text->setAlignment(osgText::Text::LEFT_CENTER);
    _textGeode->addDrawable(text.get());
    m_pSatellitePos->addChild(_textGeode);


//    osg::ref_ptr<osg::Program> program = new osg::Program;

//    program->addShader(
//                new osg::Shader(osg::Shader::VERTEX,
//                                s_OribitgetMapVertexSource()));

//    program->addShader(
//                new osg::Shader(osg::Shader::GEOMETRY,
//                                s_OribitGetMapGeometrySource()));

//    program->addShader(
//                new osg::Shader(osg::Shader::FRAGMENT,
//                                s_OribitgetMapFragmentSource()));
//    m_pOribit->getOrCreateStateSet()->setAttributeAndModes(program.get(), osg::StateAttribute::ON);

//    osg::Matrix matColor(0.0, 1.0, 0.0, 0.0,
//                                     1.0, 0.0, 0.0, 0.0,
//                                     0.0, 0.0, 1.0, 0.0,
//                                     0.0, 0.0, 0.0, 1.0);

//    osg::Matrix matColor(1.0, 0.0, 0.0, 0.0,
//                                     0.0, 1.0, 0.0, 0.0,
//                                     0.0, 0.0, 1.0, 0.0,
//                                     0.0, 0.0, 0.0, 1.0);


//    m_pOribit->getOrCreateStateSet()->addUniform(new osg::Uniform("color_matrix",matColor));
//    if(osg::Uniform* pUniform = m_pOribit->getOrCreateStateSet()->getUniform("color_matrix"))
//    {
//        pUniform->set(matColor);
//    }
    m_pOribit->getOrCreateStateSet()->setAttribute(new osg::ShadeModel);
    m_pOribit->getOrCreateStateSet()->setAttribute(new osg::LineWidth(1));
    /// 关闭光照
//    m_pOribit->getOrCreateStateSet()->setAttributeAndModes(new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA,osg::BlendFunc::ONE_MINUS_SRC_ALPHA));
    m_pOribit->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);

//    osgEarth::PhongLightingEffect * pPhong = new osgEarth::PhongLightingEffect;
//    pPhong->attach(m_pOribit->getOrCreateStateSet());
}

/// 设置卫星运动时间
void CByySatellite::SetTime(const std::vector<double> &rTime, double dStart, double dStep, double dEnd)
{
    m_vdMjd = rTime;

    /// 设置时间
    m_dStart = dStart;
    m_dStep = dStep;
    m_dEnd = dEnd;
}

#include <QDebug>
#include "Date.h"
/// 更新位置
void CByySatellite::UpdatePos(double dMJD, osg::Matrixd &rMatrix)
{
    if(fabs(m_dStep)<1e-9 || dMJD > m_dEnd)
    {
        return;
    }

    /// 时间跨度
    double dSpend = dMJD - m_dStart;

    /// 定位数据位置

    int nIndex = dSpend/m_dStep;

    /// 计算卫星当前位置
    if(nIndex >= 0)
    {
        m_nIndex = nIndex;

        CalNowPos(dMJD);

        m_bUpdateSatellite = true;
    }

//    if(!m_bCreateUnder)
//    {
//        m_pUnderCover->SetPostion(osg::Vec3d(m_stNowPos(0),m_stNowPos(1),m_stNowPos(2)));
//        m_pUnderCover->SetVDir(osg::Vec3d(m_stNowPos(3),m_stNowPos(4),m_stNowPos(5)));
//        this->addChild(m_pUnderCover->CreateNode());
//        m_bCreateUnder = true;
//    }
//    else
//    {
//        m_pUnderCover->SetPostion(osg::Vec3d(m_stNowPos(0),m_stNowPos(1),m_stNowPos(2)));
//        m_pUnderCover->SetVDir(osg::Vec3d(m_stNowPos(3),m_stNowPos(4),m_stNowPos(5)));
//    }

    osg::Matrixd localMatrix = rMatrix;
    localMatrix = localMatrix.inverse(localMatrix);

    m_vSatelliteWorldPos = localMatrix* osg::Vec3d(m_stNowPos(0),m_stNowPos(1),m_stNowPos(2));

    for(std::map<int,CSnRenderCone*>::iterator itor = m_mapCoverCon.begin();
        itor != m_mapCoverCon.end(); ++itor)
    {
        itor->second->SetSatellitePos(m_vSatelliteWorldPos);
    }


}

/// 插入地面覆盖范围
void CByySatellite::InsertCover(int nID, const std::vector<CVector> &rCover)
{
    std::map<int,CSnRenderCone*>::iterator itor = m_mapCoverCon.find(nID);

    std::vector<osg::Vec2> vCover;
    for(int i=0; i<rCover.size(); ++i)
    {
        vCover.push_back(osg::Vec2(rCover[i].GetX(),rCover[i].GetY()));
    }

    if(m_mapCoverCon.end() != itor)
    {
        itor->second->SetCover(vCover);
    }
    else
    {
        CSnRenderCone* pRenderCone = new CSnRenderCone;
        m_mapCoverCon[nID] = pRenderCone;
        pRenderCone->SetCover(vCover);
        m_pRoot->addChild(pRenderCone->CreateNode());
    }
}

/// 设置地面覆盖显示隐藏
void CByySatellite::SetUnderCover(int nID, bool bVisible)
{
    std::map<int,CSnRenderCone*>::iterator itor = m_mapCoverCon.find(nID);
    if(m_mapCoverCon.end() != itor)
    {
        itor->second->SetVisible(bVisible);
    }
}

/// 地面覆盖区域的显隐
void CByySatellite::SetUnderEarthVisible(bool bVisible)
{
    m_pUnderCover->SetVisible(bVisible);
}

/// 获取地面覆盖区域的显隐
bool CByySatellite::GetUnderEarthVisible()
{
    return(m_pUnderCover->IsVisible());
}

/// 设置圆锥显隐
void CByySatellite::SetConeVisible(bool bVisible)
{
    m_pCone->SetVisible(bVisible);
}

void CByySatellite::SetConeAngle(double dAngle)
{
    m_pCone->SetAngle(osg::DegreesToRadians(dAngle));
}

double CByySatellite::GetConeAngle()
{
    return(osg::RadiansToDegrees(m_pCone->GetAngle()));
}

/// 设置旋转角度
void CByySatellite::UpdateZiTai()
{

    static osg::Matrixd matRoll,matPitch,matYaw;

    m_pConeZiTai->setMatrix(matRoll.identity());


    osg::Quat(osg::DegreesToRadians(m_dYaw),osg::Z_AXIS).get(matYaw);
    osg::Quat(osg::DegreesToRadians(m_dPitch),osg::Y_AXIS).get(matPitch);
    osg::Quat(osg::DegreesToRadians(m_dRoll),osg::X_AXIS).get(matRoll);

    if("YRP" == m_sZiTai)
    {
        m_pConeZiTai->preMult(matYaw);
        m_pConeZiTai->preMult(matRoll);
        m_pConeZiTai->preMult(matPitch);
    }
    else if("RYP" == m_sZiTai)
    {
        m_pConeZiTai->preMult(matRoll);
        m_pConeZiTai->preMult(matYaw);
        m_pConeZiTai->preMult(matPitch);
    }
    else if("RPY" == m_sZiTai)
    {
        m_pConeZiTai->preMult(matRoll);
        m_pConeZiTai->preMult(matPitch);
        m_pConeZiTai->preMult(matYaw);
    }
    else if("PYR" == m_sZiTai)
    {
        m_pConeZiTai->preMult(matPitch);
        m_pConeZiTai->preMult(matYaw);
        m_pConeZiTai->preMult(matRoll);
    }
    else if("PRY" == m_sZiTai)
    {
        m_pConeZiTai->preMult(matPitch);
        m_pConeZiTai->preMult(matRoll);
        m_pConeZiTai->preMult(matYaw);
    }
    else
    {
        m_pConeZiTai->preMult(matYaw);
        m_pConeZiTai->preMult(matPitch);
        m_pConeZiTai->preMult(matRoll);
    }
}

void CByySatellite::SetPitchValue(double dP)
{
    m_dPitch = dP;

    UpdateZiTai();
}

void CByySatellite::SetYawValue(double dYaw)
{
    m_dYaw = dYaw;
    UpdateZiTai();
}

void CByySatellite::SetRollValue(double dRoll)
{
    m_dRoll = dRoll;
    UpdateZiTai();
}

void CByySatellite::SetZiTai(string &sZiTai)
{
    m_sZiTai = sZiTai;
    UpdateZiTai();
}

/// 获取圆锥显隐
bool CByySatellite::GetConeVisible()
{
    return(m_pCone->IsVisible());
}

#include <osgEarth/ModelLayer>

/// 地图更改
void CByySatellite::MapChanged(MAP_TYPE type)
{
    if(m_emMapType == type)
    {
        return;
    }

    m_p2DOribit->MapNodeChanged();

    this->removeChild(0,this->getNumChildren());

    switch (m_emMapType = type)
    {
    case MAP_2D:
//        m_pRoot->setNodeMask(0);
        m_pRoot->addChild(m_p2DOribit->CreateNode());
        this->removeChild(m_pOribit);
        this->removeChild(m_pSatellitePos);
        break;
    case MAP_3D:
        m_pRoot->removeChild(m_p2DOribit->CreateNode());
//        m_pRoot->setNodeMask(1);
        this->addChild(m_pOribit);
        this->addChild(m_pSatellitePos);
        break;
    }

    for(std::map<int,CSnRenderCone*>::iterator itor = m_mapCoverCon.begin();
        itor != m_mapCoverCon.end(); ++itor)
    {
        itor->second->MapChanged(type);
    }
}

/// 更新名字
void CByySatellite::UpdateName(const std::string &sName)
{
    m_sName = QString::fromLocal8Bit(sName.c_str());
    text->setText(m_sName.toUtf8().data(),osgText::String::ENCODING_UTF8);
}

void CByySatellite::SetConeColor(const osg::Vec4 &vColor)
{
    m_pCone->SetColor(vColor);
}

void CByySatellite::Scale(double dScale)
{
    m_pSatellite->setMatrix(osg::Matrix::scale(dScale,dScale,dScale));
}


/// 计算当前卫星位置
void CByySatellite::CalNowPos(double dMJD)
{
    static double dTime[SIZE];

    /// 过滤掉非法值
    if(1 > m_nIndex)
    {
        m_nIndex = 1;
    }
    else if(m_nIndex > m_vdMjd.size() -1)
    {
        m_nIndex = m_vdMjd.size() - 1;
    }


    /// 赋值
    dTime[0] = m_vdMjd[m_nIndex-1];
    dTime[1] = m_vdMjd[m_nIndex];
    dTime[2] = m_vdMjd[m_nIndex+1];

    /// 计算插值
    m_stNowPos(0) = CalItNewton(dTime,dMJD,0);
    m_stNowPos(1) = CalItNewton(dTime,dMJD,1);
    m_stNowPos(2) = CalItNewton(dTime,dMJD,2);
    m_stNowPos(3) = CalItNewton(dTime,dMJD,3);
    m_stNowPos(4) = CalItNewton(dTime,dMJD,4);
    m_stNowPos(5) = CalItNewton(dTime,dMJD,5);

    /// 更新
    Math::CVector tmpVector = m_stNowPos.slice(0,2);
    m_pCone->SetDistance(tmpVector.Length());
//    m_pCone->SetVisible(false);
}

/// 计算插值
double CByySatellite::CalItNewton(double *dX, double dT, int nDim)
{
    static double dY[SIZE];
    dY[0] = m_vOribit[m_nIndex-1](nDim);
    dY[1] = m_vOribit[m_nIndex](nDim);
    dY[2] = m_vOribit[m_nIndex+1](nDim);

    return(Numerical::Cntpol::ItNewton(SIZE,dX,dY,dT));
}
