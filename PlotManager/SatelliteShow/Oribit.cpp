#include <osg/Geometry>
#include "Oribit.h"

/// 卫星轨道更新
class OribitUpdateCallBack : public osg::NodeCallback
{
public:
    OribitUpdateCallBack(COribit* pSatellite,osg::Geometry* pGeometry)
        :m_pSatellite(pSatellite),m_pNode(pGeometry){}

    /// 更新
    void operator ()(osg::Node *node, osg::NodeVisitor *nv)
    {
        if(m_pSatellite->m_bUpdate)
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

            m_pSatellite->m_bUpdate = false;
        }

        traverse(node,nv);
    }

private:
    COribit* m_pSatellite; /// 数据指针
    osg::Geometry* m_pNode; /// node指针
};

COribit::COribit(ISceneGraph *pSceneGraphe):
    QtOsgSceneNode<IOribit>(pSceneGraphe)
{
}

void COribit::SetJ2000Oribit(const vector<CVector> & vOribit)
{
    m_vOribit = vOribit;
    m_bUpdate = true;
}

void COribit::SetGeoOribit(const vector<CVector> &)
{
}

void COribit::SetOribitColor(const SceneColor &)
{
}

void COribit::InitSceneNode()
{
    m_pOsgNode = new osg::Group;
    m_pOribit = new osg::Geometry();
    m_pOribit->setUpdateCallback(new OribitUpdateCallBack(this,m_pOribit));
    m_pOsgNode->addChild(m_pOribit);
}
