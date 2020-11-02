#include <QWidget>
#include "QtWindow.h"
#include "QtFBOWindow.h"
#include "Widget/QtOsgWindow.h"
#include "SceneGraph/QtViewPort.h"
#include "SceneGraph/QtRender.h"

QtWindow::QtWindow(ISceneGraph *pSceneGraph, QtRender *pRender, QThread* pThread):
    m_pSceneGraph(pSceneGraph),
    m_pThread(pThread),
    m_pRender(pRender),
    m_pFBOWindow(new QtFBOWindow(&m_allWindowMessageObserver)),
    m_pMainViewPoint(new QtViewPort(pRender,pSceneGraph))
{
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
        m_pWindow = new QtOsgWindow(m_pThread);
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
        m_pWindow = new QtOsgWindow(m_pThread);
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
        m_pMainViewPoint->GetOsgView()->getCamera()->setSmallFeatureCullingPixelSize(-1.0f);
        m_bInit=true;
    }
}
