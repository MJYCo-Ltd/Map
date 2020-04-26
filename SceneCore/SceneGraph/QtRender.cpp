#include <osg/OperationThread>
#include "QtRender.h"
#include "Window/QtFBOWindow.h"
#include "Window/Widget/QtOsgWindow.h"
#include "Qml/QtOsgItem.h"

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

QtRender::QtRender(QObject *parent) :
    QObject(parent)
{
    m_pOsgViewer = new osgViewer::CompositeViewer;
    m_pOsgViewer->setKeyEventSetsDone(0);
    m_pOsgViewer->setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
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
        m_pSender = sender();
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
    if(auto pItem = qobject_cast<QtOsgItem*>(pRenderResize->GetSender()))
    {
        pItem->GetFBOWindow()->ReSize(pRenderResize->GetSize());
    }
    else if(auto pWindow = qobject_cast<QtOsgWindow*>(pRenderResize->GetSender()))
    {
        pWindow->GetFBOWindow()->ReSize(pRenderResize->GetSize());
    }
}

/// 渲染引擎
void QtRender::Render()
{
    m_pOsgViewer->frame();
    emit(RenderAFrame());
}

/// 释放资源
void QtRender::Release()
{
    m_pOsgViewer->done();
}

RenderResize::RenderResize(QObject *pSender, const QSize &rSize):
    QEvent(static_cast<Type>(RENDER_RESIZE)),
    m_pSender(pSender),
    m_stSize(rSize)
{
}
