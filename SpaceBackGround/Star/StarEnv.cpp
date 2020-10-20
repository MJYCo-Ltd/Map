#include <osg/BlendFunc>
#include <osg/Depth>
#include <osgEarth/VirtualProgram>
#include <osgGA/GUIEventHandler>

#include <Math/Matrix.h>
#include <Satellite/CoorSys.h>
#include <Satellite/IRESInfo.h>
#include <Satellite/Date.h>

#include <ISceneCore.h>
#include <SceneGraph/ISceneGraph.h>
#include <Inner/ILoadResource.h>
#include <Inner/IOsgViewPoint.h>

#include "StarEnv.h"
#include "Milkyway.h"
#include "Boundary.h"
#include "StarManager.h"

/// 响应窗口变化消息
class ResizeEventHandler:public osgGA::GUIEventHandler
{
public:
    ResizeEventHandler(CStarEnv* pParent)
        :m_pParent(pParent){}

    bool handle(const osgGA::GUIEventAdapter& ea,
        osgGA::GUIActionAdapter& aa)
    {
        /// 只关注窗口更新消息
        if( osgGA::GUIEventAdapter::RESIZE == ea.getEventType())
        {
            double dWidth = ea.getWindowWidth();
            double dHeight = ea.getWindowHeight();

            dWidth = dWidth < 1 ? 1 : dWidth;
            dHeight = dHeight < 1 ? 1 : dHeight;

            double aspectRatio = dWidth/dHeight;
            m_pParent->setViewport( new osg::Viewport(0, 0, dWidth, dHeight));
            m_pParent->setProjectionMatrixAsPerspective(45.0,aspectRatio,0.00011,1.1);
        }

        return osgGA::GUIEventHandler::handle(ea,aa);
    }
private:
    CStarEnv* m_pParent;
};

/// 空间背景
CStarEnv::CStarEnv(ISceneGraph *pSceneGraph):
    m_pSceneGraph(pSceneGraph)
{
    /// 初始化IERS文件
    if(!Aerospace::CIRESInfo::GetInstance()->IsInit())
    {
        Aerospace::CIRESInfo::GetInstance()->Init(GetExePath() + "SpaceResource/dynamics/finals2000A.data");
    }

    /// 优先渲染
    setRenderOrder(osg::Camera::PRE_RENDER);
    setAllowEventFocus(false);
    setReferenceFrame(osg::Transform::ABSOLUTE_RF);

    /// 不进行远近裁剪面的计算
    setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
    setProjectionMatrixAsPerspective(45.0,1.0,0.00011,1.1);

    osg::StateSet *state = getOrCreateStateSet();


    state->setMode(GL_BLEND, osg::StateAttribute::ON);
    state->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

    state->setAttributeAndModes(
                new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA,
                                   osg::BlendFunc::ONE_MINUS_SRC_ALPHA),
                osg::StateAttribute::ON);

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
    setViewport(pMainView->getCamera()->getViewport());
    m_pMainView = pMainView;
    m_pMainCamera = m_pMainView->getCamera();
    setViewport(m_pMainCamera->getViewport());
    m_pMainCamera->setEventCallback(new ResizeEventHandler(this));
}


void CStarEnv::traverse(osg::NodeVisitor& nv)
{
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

void CStarEnv::UpdateMatrix(const CMatrix &rRotate)
{
    m_rRoate.set(rRotate(0, 0), rRotate(0, 1), rRotate(0, 2), 0.,
                 rRotate(1, 0), rRotate(1, 1), rRotate(1, 2), 0.,
                 rRotate(2, 0), rRotate(2, 1), rRotate(2, 2), 0.,
                 0., 0., 0., 1.);
}

CStarEnv::~CStarEnv()
{
}
