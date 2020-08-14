#include <osg/NodeCallback>
#include <osg/Matrix>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osg/BlendFunc>

#include <SceneGraph/ISceneGraph.h>
#include <Inner/ILoadResource.h>
#include "Satellite3D.h"

/// 卫星位置回调函数
class SatelliteUpdateCallBack:public osg::NodeCallback
{
public:
    SatelliteUpdateCallBack(CSatellite3D* pSatellite,osg::MatrixTransform* pNode):
        m_pSatellite(pSatellite),m_pSatelliteNode(pNode) {}

    /// 更新
    bool run(osg::Object* object, osg::Object* data)
    {
        if(m_pSatellite->m_bUpdateSatellite)
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

        return traverse(object, data);
    }

    /**
     * @brief 获取矩阵
     * @return
     */
    Math::CMatrix& Matrix(){return(m_matTransla);}
private:
    CSatellite3D*               m_pSatellite;   /// 卫星管理类
    osg::MatrixTransform* m_pSatelliteNode;/// 卫星位置类
    Math::CMatrix             m_matTransla;     /// 平移矩阵
};

/// 卫星轨道更新
class OribitUpdateCallBack : public osg::NodeCallback
{
public:
    OribitUpdateCallBack(CSatellite3D* pSatellite,osg::Geometry* pGeometry)
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
    CSatellite3D* m_pSatellite; /// 数据指针
    osg::Geometry* m_pNode; /// node指针
};

CSatellite3D::CSatellite3D(ISceneGraph *pSceneGraph):
    m_pSceneGraph(pSceneGraph)
{
}

/// 设置卫星名称
void CSatellite3D::SetSatName(const string & satName)
{
    m_pName->setText(satName,osgText::String::ENCODING_UTF8);
}

/// 设置模型路径
void CSatellite3D::SetModelPath(const string & sModelPath)
{
}

void CSatellite3D::SetOribitColor(const SceneColor &)
{
}

/// 构建名称
void CSatellite3D::BuildName()
{
    osg::Geode* _textGeode = new osg::Geode;
    _textGeode->setCullingActive(false);
    osg::StateSet *state = _textGeode->getOrCreateStateSet();
    state->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    state->setTextureMode(0, GL_TEXTURE_2D, osg::StateAttribute::ON);
    state->setAttributeAndModes(
                new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA,
                                   osg::BlendFunc::ONE_MINUS_SRC_ALPHA),
                osg::StateAttribute::ON);
    m_pName= new osgText::Text;
    m_pName->setFontResolution(40, 40);

    m_pName->setFont(m_pSceneGraph->ResouceLoader()->LoadFont("fonts/msyh.ttf"));
    m_pName->setText("hello",osgText::String::ENCODING_UTF8);
    m_pName->setColor(osg::Vec4(0,1.0,0.0,0.8));
    m_pName->setCharacterSize(20);
    m_pName->setAxisAlignment(osgText::Text::SCREEN);
    m_pName->setCharacterSizeMode(osgText::Text::SCREEN_COORDS);
    m_pName->setAutoRotateToScreen(true);
    m_pName->setPosition(osg::Vec3(0, 0, 0));
    m_pName->setAlignment(osgText::Text::LEFT_CENTER);
    _textGeode->addDrawable(m_pName.get());
    this->addChild(_textGeode);
}
