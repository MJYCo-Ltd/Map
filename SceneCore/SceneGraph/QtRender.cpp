#include <osg/OperationThread>
#include <Inner/OsgExtern/OsgExtern.h>
#include "QtRender.h"
#include "Window/QtFBOWindow.h"

class CModifyView:public osg::Operation
{
public:
    CModifyView(osgViewer::CompositeViewer* pViewer, osgViewer::View* pView,bool bAdd)
        :m_pViewer(pViewer)
        ,m_pView(pView)
        ,m_bAdd(bAdd){}

    void operator()(osg::Object*)
    {
        if(m_bAdd)
        {
            m_pViewer->addView(m_pView);
        }
        else
        {
            m_pViewer->removeView(m_pView);
        }
    }
private:
    osg::ref_ptr<osgViewer::CompositeViewer> m_pViewer;      /// 父节点
    osg::ref_ptr<osgViewer::View>  m_pView;
    bool                     m_bAdd; /// 是否增加
};

/// 渲染
QtRender::QtRender(QObject *parent) :
    QObject(parent),
    m_pOsgViewer(new osgViewer::CompositeViewer)
{
    m_pOsgViewer->addUpdateOperation(new CRenderThreadCall(this));
    m_pOsgViewer->setKeyEventSetsDone(0);
    m_pOsgViewer->setThreadingModel(osgViewer::ViewerBase::CullThreadPerCameraDrawSingle);
}

QtRender::~QtRender()
{
    Release();
}

/// 增加视图
void QtRender::AddView(osgViewer::View *pView)
{
    m_pOsgViewer->addUpdateOperation(new CModifyView(m_pOsgViewer.get(),pView,true));
}

/// 移除视图
void QtRender::RemoveView(osgViewer::View *pView)
{
    m_pOsgViewer->addUpdateOperation(new CModifyView(m_pOsgViewer.get(),pView,false));
}

/// 添加更新操作
void QtRender::AddUpdateOperation(osg::Operation *pUpdate)
{
    m_pOsgViewer->addUpdateOperation(pUpdate);
}

/// 重新构建线程
void QtRender::ResetupThread()
{
    m_bResetThread=true;
}

/// 订阅消息
void QtRender::SubMessage(RenderCall *pRenderCall)
{
    m_setMessage.Add(pRenderCall);
}

/// 更新渲染
void QtRender::UpdateRender()
{
    /// 当线程需要修改时
    if(m_bResetThread)
    {
        if (m_pOsgViewer->areThreadsRunning())
        {
            m_pOsgViewer->stopThreading();
        }

        if(osgViewer::ViewerBase::SingleThreaded != m_pOsgViewer->getThreadingModel())
        {
            m_pOsgViewer->setUpThreading();
        }

        m_bResetThread=false;
    }
    static std::vector<RenderCall*> allMessage;
    m_setMessage.Take(allMessage);
    /// 遍历所有的订阅者
    for(auto one:allMessage)
    {
        one->FrameCall();
        if(!one->m_bCallOne)
        {
            m_setMessage.Add(one);
        }
    }
    allMessage.clear();
}

/// 响应发送的消息
bool QtRender::event(QEvent *e)
{
    switch (int(e->type()))
    {
    case RENDER_INIT:
        return true;
    case RENDER_START:
        Render();
        return true;
    case RENDER_RESIZE:
        Resize(static_cast<RenderResize*>(e));
        return true;
    case RENDER_STOP:
        Release();
        return true;
    default:
        return QObject::event(e);
    }
}

/// 重置窗口
void QtRender::Resize(RenderResize* pRenderResize)
{
    pRenderResize->GetSender()->ReSize(pRenderResize->GetSize());
}

/// 渲染引擎
void QtRender::Render()
{
    m_pOsgViewer->frame();
    emit RenderAFrame();
}

/// 释放资源
void QtRender::Release()
{
    m_pOsgViewer->done();
}

RenderResize::RenderResize(QtFBOWindow *pFBOWindow, const QSize &rSize):
    QEvent(static_cast<Type>(RENDER_RESIZE)),
    m_pFBOWindow(pFBOWindow),
    m_stSize(rSize)
{
}
