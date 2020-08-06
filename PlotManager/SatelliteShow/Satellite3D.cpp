#include <osg/NodeCallback>
#include <osg/Matrix>
#include <osg/MatrixTransform>
#include <osg/Geometry>

#include "Satellite3D.h"

/// 卫星位置回调函数
class SatelliteUpdateCallBack:public osg::NodeCallback
{
public:
    SatelliteUpdateCallBack(CSatellite3D* pSatellite,osg::MatrixTransform* pNode):
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

CSatellite3D::CSatellite3D()
{
}
