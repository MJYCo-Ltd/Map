#include <osg/BlendFunc>
#include <osg/Depth>
#include <osgEarth/CullingUtils>
#include <osgGA/GUIEventHandler>
#include <osgEarth/VirtualProgram>
#include <Math/Matrix.h>
#include <Satellite/CoorSys.h>
#include <Satellite/IRESInfo.h>
#include <Satellite/Date.h>

#include <ISceneCore.h>
#include <SceneGraph/ISceneGraph.h>
#include <Inner/ILoadResource.h>
#include <Inner/IOsgViewPoint.h>
#include <SceneGraph/IWindow.h>
#include <SceneGraph/IViewPort.h>

#include "StarEnv.h"
#include "Milkyway.h"
#include "Boundary.h"
#include "StarManager.h"

/// 空间背景
CStarEnv::CStarEnv(ISceneGraph *pSceneGraph):
    m_pSceneGraph(pSceneGraph)
{
    /// 优先渲染
    setRenderOrder(osg::Camera::PRE_RENDER);
    setAllowEventFocus(false);
    setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /// 不进行远近裁剪面的计算
    setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);

    osg::StateSet *state = getOrCreateStateSet();
    state->setGlobalDefaults();

    auto pSpaceEnvStateSet = pSceneGraph->ResouceLoader()->GetOrCreateStateSet(SPACEENV_STATE);
    state->merge(*pSpaceEnvStateSet);

    state->setMode(GL_BLEND,osg::StateAttribute::ON);

    state->setAttributeAndModes(
                new osg::Depth(osg::Depth::LESS, 0, 1, false),
                osg::StateAttribute::OFF);


    m_pMilkyway = new CMilkyway(m_pSceneGraph);
    m_pBoundary = new  CBoundary();
    m_pStarManager = new CStarManager(m_pSceneGraph);

    m_pStarManager->ReadStar(-1,BIN);

    this->addChild(m_pMilkyway->getNode());
    this->addChild(m_pBoundary->getNode());
    this->addChild(m_pStarManager->getNode());

    SetMainView(dynamic_cast<IOsgViewPoint*>(m_pSceneGraph->GetMainWindow()->GetMainViewPoint())->GetOsgView());
}

void CStarEnv::SetMainView(osgViewer::View* pMainView)
{
    m_pMainView = pMainView;
    m_pMainCamera = m_pMainView->getCamera();
    setViewport(m_pMainCamera->getViewport());
    setGraphicsContext(m_pMainCamera->getGraphicsContext());
    InitProjMatrix();
}


void CStarEnv::traverse(osg::NodeVisitor& nv)
{
    if(!m_bInit)
    {
        InitProjMatrix();
    }

    if (m_pMainCamera.valid() && osg::NodeVisitor::CULL_VISITOR == nv.getVisitorType())
    {
        osg::Matrix matrix = m_pMainCamera->getViewMatrix();
        matrix.getRotate().get(matrix);
        this->setViewMatrix(m_rRoate*matrix);
    }

    osg::Camera::traverse( nv );
}

/// 设置星区是否可见
void CStarEnv::SetBoundaryVisible(bool bVisible)
{
    m_pBoundary->SetVisible(bVisible);
}

/// 设置恒星名称可见
void CStarEnv::SetStarNameVisible(bool bVisible)
{
    m_pStarManager->SetStarNameVisible(bVisible);
}

/// 设置星座连线可见
void CStarEnv::SetConstellationVisible(bool bVisible)
{
    m_pStarManager->SetConstellation(bVisible);
}

/// 设置银河的显隐
void CStarEnv::SetMilkwayVisible(bool bVisible)
{
    m_pMilkyway->SetVisible(bVisible);
}

void CStarEnv::UpdateMatrix(const Math::CMatrix &rRotate)
{
    m_rRoate.set(rRotate(0, 0), rRotate(0, 1), rRotate(0, 2), 0.,
                 rRotate(1, 0), rRotate(1, 1), rRotate(1, 2), 0.,
                 rRotate(2, 0), rRotate(2, 1), rRotate(2, 2), 0.,
                 0., 0., 0., 1.);
}

void CStarEnv::InitProjMatrix()
{
    if(!osg::equivalent(m_pMainCamera->getProjectionMatrix()(3,3),1.0))
    {
        double fovy,aspectRatio,zNear,zFar;
        m_pMainCamera->getProjectionMatrixAsPerspective(fovy,aspectRatio,zNear,zFar);
        setProjectionMatrixAsPerspective(fovy,aspectRatio,0.00011,1.1);
        m_bInit = true;
    }
}

CStarEnv::~CStarEnv()
{
}
