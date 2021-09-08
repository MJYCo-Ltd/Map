#include "OsgItem.h"
#include "OsgRender.h"

#include "TrajectoryShow.h"
#include <osgEarth/Style>
#include <osgEarth/LineSymbol>
#include <osgEarth/PointSymbol>
#include <osgEarth/LabelNode>
#include <osg/LineWidth>

#include "OsgUpdateOperation.h"
CTrajectoryShow::CTrajectoryShow()
{
    m_pRoot =new osg::Group;
}

/// 设置轨道
void CTrajectoryShow::SetTrajectory(const std::vector<Math::CVector> &vTrajectory)
{
    m_vAllData = vTrajectory;


    bool bAdd=false;
    if(m_Oribit.valid())
    {
        QOsgItem::GetInstance()->AddOperation(new COsgRemoveNodeOperation(m_pRoot,m_Oribit));
        bAdd = true;
    }

    osg::ref_ptr<osg::Vec3Array> pArray = new osg::Vec3Array;
    for(const Math::CVector& rPos : vTrajectory)
    {
        pArray->push_back(osg::Vec3(rPos(1),rPos(2),rPos(3)));
    }

    /// 弹道轨道
    m_Oribit = new osg::Geometry();

    osg::ref_ptr<osg::Vec4Array> pColor = new osg::Vec4Array;
    pColor->push_back(osg::Vec4(255./255.,0,0,1.f));

    /// 绑定顶点数组
    m_Oribit->setVertexArray(pArray);

    /// 绑定颜色数组
    m_Oribit->setColorArray(pColor,osg::Array::BIND_PER_PRIMITIVE_SET);


    m_Oribit->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP,0,pArray->size()));


    QOsgItem::GetInstance()->AddOperation(new COsgAddNodeOperation(m_pRoot,m_Oribit));
}

osg::Node *CTrajectoryShow::GetNode()
{
    return(m_pRoot.get());
}
