#include <QWidget>
#include "QtWindow.h"
#include "QtFBOWindow.h"
#include "Widget/QtOsgWindow.h"
#include "SceneGraph/QtViewPort.h"
#include "SceneGraph/QtRender.h"

bool ViewEventCallback::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &, osg::Object *, osg::NodeVisitor *)
{
    if(ea.MOVE == ea.getEventType() || ea.SCROLL == ea.getEventType())
    {
        if (ea.getNumPointerData()>=1)
        {
            const osgGA::PointerData* pd = ea.getPointerData(ea.getNumPointerData()-1);
            const osg::Camera* camera = pd->object.valid() ? pd->object->asCamera() : 0;
            if (camera)
            {
                osg::Vec3d startVertex = osg::Vec3d(pd->getXnormalized(), pd->getYnormalized(),-1);
                osg::Vec3d endVertex = osg::Vec3d(pd->getXnormalized(), pd->getYnormalized(),1);

                osg::ref_ptr< osgUtil::LineSegmentIntersector > picker =
                        new osgUtil::LineSegmentIntersector(osgUtil::Intersector::PROJECTION, startVertex, endVertex);

                ///只取最近的
                picker->setIntersectionLimit( osgUtil::Intersector::LIMIT_NEAREST );

                osgUtil::IntersectionVisitor iv(picker.get());
                const_cast<osg::Camera*>(camera)->accept(iv);

                osg::Vec3d out_coords;
                if (picker->containsIntersections())
                {
                    out_coords = picker->getIntersections().begin()->getWorldIntersectPoint();
                }
                else
                {
                    out_coords = endVertex * osg::Matrixd::inverse(camera->getProjectionMatrix());
                }
                QMetaObject::invokeMethod(m_pWindow,"MouseMovePos",Q_ARG(double,out_coords.x()),
                                          Q_ARG(double,out_coords.y()),Q_ARG(double,out_coords.z()));
            }
        }
    }
    return(false);
}

QtWindow::QtWindow(ISceneGraph *pSceneGraph, QtRender *pRender, QThread* pThread, int nType):
    m_pSceneGraph(pSceneGraph),
    m_pThread(pThread),
    m_pRender(pRender),
    m_pFBOWindow(new QtFBOWindow(&m_allWindowMessageObserver)),
    m_pMainViewPoint(new QtViewPort(pRender,pSceneGraph)),
    m_nType(nType)
{
    m_pViewEventCallback = new ViewEventCallback(this);
}

QtWindow::~QtWindow()
{
    for(auto one : m_vOtherViewPoint)
    {
        delete one;
    }
    m_vOtherViewPoint.clear();
}

/// 设置帧率
void QtWindow::SetFrameRate(int nRate)
{
    if(nRate >0 && m_nFrameRate != nRate)
    {
        m_nFrameRate = nRate;
        if(nullptr != m_pWindow)
        {
            m_pWindow->ChangeTimer(1000/m_nFrameRate);
        }
    }
}

IViewPort *QtWindow::GetMainViewPoint()
{
    return(m_pMainViewPoint);
}



/// 创建一个新的节点
IViewPort *QtWindow::CreateViewPoint()
{
    auto newOne = new QtViewPort(m_pRender,m_pSceneGraph);

    /// 创建新的视图
    auto pView = newOne->GetOsgView();
    pView->getCamera()->setGraphicsContext(m_pFBOWindow);
    pView->addEventHandler(m_pViewEventCallback);

    m_pRender->AddView(pView);

    m_vOtherViewPoint.push_back(newOne);

    return(newOne);
}

/// 删除视点节点
bool QtWindow::DeleteViewPoint(IViewPort *& rViewPoint)
{
    auto findResult = find(m_vOtherViewPoint.begin(),m_vOtherViewPoint.end(),rViewPoint);
    if(findResult != m_vOtherViewPoint.end())
    {
        auto qtViewPoint = static_cast<QtViewPort*>(rViewPoint);
        m_pRender->RemoveView(qtViewPoint->GetOsgView());
        delete qtViewPoint;
        rViewPoint = nullptr;
        m_vOtherViewPoint.erase(findResult);
        return(true);
    }

    return(false);
}


/// 转换成QWidget
QWidget *QtWindow::ToWidget()
{
    if(m_bCanChange)
    {
        m_pWindow = new QtOsgWindow(m_pThread,m_nType);
        m_pWindow->ChangeTimer(1000/m_nFrameRate);

        m_pWindow->ConnectRender(m_pRender);
        m_pWindow->SetFBOWindow(m_pFBOWindow);
        m_pWidget = QWidget::createWindowContainer(m_pWindow);
        m_bCanChange = false;
    }

    return(m_pWidget);
}

/// 转成QWindow
QWindow *QtWindow::ToWindow()
{
    if(m_bCanChange)
    {
        m_pWindow = new QtOsgWindow(m_pThread,m_nType);
        m_pWindow->ChangeTimer(1000/m_nFrameRate);

        m_pWindow->ConnectRender(m_pRender);
        m_pWindow->SetFBOWindow(m_pFBOWindow);
        m_bCanChange = false;
    }

    return(m_pWindow);
}

bool QtWindow::SubMessage(IWindowMessageObserver *pObserver)
{
    auto findResult = find(m_allWindowMessageObserver.begin(),m_allWindowMessageObserver.end(),pObserver);
    if(findResult == m_allWindowMessageObserver.end())
    {
        m_allWindowMessageObserver.push_back(pObserver);
    }

    return(true);
}

bool QtWindow::UnSubMessage(IWindowMessageObserver *pObserver)
{
    auto findResult = find(m_allWindowMessageObserver.begin(),m_allWindowMessageObserver.end(),pObserver);
    if(findResult != m_allWindowMessageObserver.end())
    {
        m_allWindowMessageObserver.erase(findResult);
        return(true);
    }
    else
    {
        return(false);
    }
}

/// 初始化窗口
void QtWindow::InitWindow()
{
    if(!m_bInit)
    {
        m_pMainViewPoint->GetOsgView()->getCamera()->setGraphicsContext(m_pFBOWindow);
        m_pMainViewPoint->GetOsgView()->getCamera()->setNearFarRatio(0.0001);
        m_pMainViewPoint->GetOsgView()->getCamera()->setSmallFeatureCullingPixelSize(1.0f);
        m_pMainViewPoint->GetOsgView()->addEventHandler(m_pViewEventCallback);
        m_bInit=true;
    }
}

/// 鼠标移动位置消息
void QtWindow::MouseMovePos(double dX, double dY, double dZ)
{
    static ScenePos s_stWordPos;
    auto iter = m_allWindowMessageObserver.begin();
    auto tmpIter = iter;

    s_stWordPos.fX = dX;
    s_stWordPos.fY = dY;
    s_stWordPos.fZ = dZ;

    for(;iter != m_allWindowMessageObserver.end();)
    {
        ++tmpIter;
        (*iter)->MovePos(s_stWordPos);
        iter = tmpIter;
    }
}
