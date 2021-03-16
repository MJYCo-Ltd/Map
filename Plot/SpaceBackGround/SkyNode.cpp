#include <osgUtil/CullVisitor>
#include "SkyNode.h"
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
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

/// 更新时间
void CSkyNode::UpdateDate(double dMJD)
{
    if(fabs(m_dMJD - dMJD) > 1e-11)
    {
        m_dMJD = dMJD;
        m_pSolarEnv->UpdateTime(m_dMJD);
    }
}

void CSkyNode::UpdateMatrix(const Math::CMatrix &rRotate)
{
    m_pStarEnv->UpdateMatrix(rRotate);
}

void CSkyNode::ShowAtmosphere(bool bVisble)
{
    m_pSolarEnv->ShowAtmosphere(bVisble);
}

void CSkyNode::traverse(osg::NodeVisitor & nv)
{
    if (nv.getVisitorType() == nv.CULL_VISITOR)
    {
        osgUtil::CullVisitor* cv = nv.asCullVisitor();

        bool needToRestoreInheritanceMask =
                (cv->getInheritanceMask() & osg::CullSettings::CLAMP_PROJECTION_MATRIX_CALLBACK) > 0;

        /// 保存原有的计算投影矩阵的回调
        osg::ref_ptr<osg::CullSettings::ClampProjectionMatrixCallback> cb =
                cv->getClampProjectionMatrixCallback();

        cv->setClampProjectionMatrixCallback(0L);

        /// 真实的节点进行遍历
        m_pSpaceBackGroundRoot->accept( nv );

        if(cb.valid())
        {
            cv->setClampProjectionMatrixCallback(cb.get());
        }

        if(needToRestoreInheritanceMask)
        {
            cv->setInheritanceMask(
                        cv->getInheritanceMask()|osg::CullSettings::CLAMP_PROJECTION_MATRIX_CALLBACK);
        }
    }

    if(nv.getVisitorType() == nv.UPDATE_VISITOR)
    {
        m_pSpaceBackGroundRoot->accept(nv);
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
