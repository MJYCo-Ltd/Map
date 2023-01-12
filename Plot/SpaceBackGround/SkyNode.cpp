#include <osgUtil/CullVisitor>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include "SkyNode.h"
CSkyNode::CSkyNode(ISceneGraph *pSceneGraph)
{
    m_pSpaceBackGroundRoot = new osg::Group;

    m_pStarEnv = new CStarEnv(pSceneGraph);
    m_pSolarEnv = new CSolarEnv(pSceneGraph);
    m_pSolarEnv->CreateSolar();

    m_pSpaceBackGroundRoot->addChild(m_pStarEnv);
    m_pSpaceBackGroundRoot->addChild(m_pSolarEnv);
}

void CSkyNode::SetMaxVisibleMagnitude(int nMax)
{
}

void CSkyNode::SetMilkywayVisible(bool bShow)
{
    m_pStarEnv->SetMilkwayVisible(bShow);
}

void CSkyNode::SetStarNamesVisible(bool bShow)
{
    m_pStarEnv->SetStarNameVisible(bShow);
}

void CSkyNode::SetBoundariesVisible(bool bShow)
{
    m_pStarEnv->SetBoundaryVisible(bShow);
}

void CSkyNode::SetConstellationLinesVisible(bool bShow)
{
    m_pStarEnv->SetConstellationVisible(bShow);
}

/// 设置显示
void CSkyNode::SetConstellationNamesVisible(bool bShow)
{
    m_pStarEnv->SetStarNameVisible(bShow);
}

/// 设置行星名称显隐
void CSkyNode::SetPlanetsNamesVisible(bool bShow)
{
    m_pSolarEnv->SetPlanetNameShow(bShow);
}


void CSkyNode::UpdateMatrix(const Math::CMatrix &rRotate)
{
    m_pStarEnv->UpdateMatrix(rRotate);
}

void CSkyNode::UpdatePos(const std::vector<Math::CVector> &vSolarPos)
{
    m_pSolarEnv->UpdatePos(vSolarPos);
}

void CSkyNode::traverse(osg::NodeVisitor & nv)
{
    if (nv.getVisitorType() == nv.CULL_VISITOR
            ||nv.getVisitorType() == nv.UPDATE_VISITOR)
    {
        /// 真实的节点进行遍历
        m_pSpaceBackGroundRoot->accept( nv );
    }

    osg::Group::traverse(nv);
}

osg::BoundingSphere CSkyNode::computeBound() const
{
    return osg::BoundingSphere();
}

void CSkyNode::releaseGLObjects(osg::State *state) const
{
    osg::Group::releaseGLObjects(state);
    m_pSpaceBackGroundRoot->releaseGLObjects(state);
}

void CSkyNode::resizeGLObjectBuffers(unsigned maxSize)
{
    osg::Group::resizeGLObjectBuffers(maxSize);
    m_pSpaceBackGroundRoot->resizeGLObjectBuffers(maxSize);
}
