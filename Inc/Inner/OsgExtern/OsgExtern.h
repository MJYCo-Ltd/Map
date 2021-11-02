#ifndef OSGEXTERN_HEADER_H
#define OSGEXTERN_HEADER_H
#include <osg/OperationThread>
#include <osg/Group>
#include <osgEarth/Controls>

/**
 * @brief 将显示节点从场景中移除
 */
class RemoveFromeScene:public osg::Operation
{
public:
    RemoveFromeScene(osg::Node* pParent):m_pParent(pParent){}

    /**
     * @brief 回调
     */
    void operator()(osg::Object*)
    {
        if(m_pParent.valid())
        {
            if(auto pGroup = m_pParent->asGroup())
            {
                pGroup->removeChildren(0,pGroup->getNumChildren());
            }

            auto parents = m_pParent->getParents();
            for(auto one : parents)
            {
                one->removeChild(m_pParent);
            }
        }
    }
private:
    osg::ref_ptr<osg::Node> m_pParent;
};

/**
 * @brief 清空所有子节点
 */
class CClearChildNode:public osg::Operation
{
public:
    CClearChildNode(osg::Group* pParent):m_pParent(pParent){}
    void operator()(osg::Object*)
    {
        if(m_pParent.valid())
        {
            m_pParent->removeChildren(0,m_pParent->getNumChildren());
        }
    }
private:
    osg::ref_ptr<osg::Group> m_pParent; /// 父节点
};

/**
 * @brief 修改节点数据
 */
class CModifyNode:public osg::Operation
{
public:
    CModifyNode(osg::Group* pParent,osg::Node* pChild,
                bool bAdd):m_pParent(pParent)
      ,m_pChild(pChild),m_bAdd(bAdd){}

    void operator()(osg::Object*)
    {
        if(m_pParent.valid())
        {
            if(m_bAdd)
            {
                m_pParent->addChild(m_pChild);
            }
            else
            {
                m_pParent->removeChild(m_pChild);
            }
        }
    }
private:
    osg::ref_ptr<osg::Group> m_pParent; /// 父节点
    osg::ref_ptr<osg::Node>  m_pChild;  /// 子节点
    bool                     m_bAdd;    /// 是否增加
};

/**
 * @brief 替换节点
 */
class CReplaceNode:public osg::Operation
{
public:
    CReplaceNode(osg::Node* pOldNode,osg::Node* pNewNode):m_pOldNode(pOldNode),m_pNewNode(pNewNode){}

    void operator()(osg::Object*)
    {
        if(m_pOldNode.valid())
        {
            auto parentList = m_pOldNode->getParents();
            for(auto one : parentList)
            {
                one->setChild(one->getChildIndex(m_pOldNode),m_pNewNode);
            }
        }
    }
private:
    osg::ref_ptr<osg::Node> m_pOldNode;
    osg::ref_ptr<osg::Node> m_pNewNode;
};

/**
 * @brief 修改界面控制节点
 */
class CModifyControl:public osg::Operation
{
public:
    CModifyControl(osgEarth::Controls::ControlCanvas* pCanvas
                   ,osgEarth::Controls::Control*pControl,bool bAdd):
        m_pCanvas(pCanvas),m_pControl(pControl),m_bAdd(bAdd){}

    void operator()(osg::Object*)
    {
        if(m_pCanvas.valid())
        {
            if(m_bAdd)
            {
                m_pCanvas->addControl(m_pControl.get());
            }
            else
            {
                m_pCanvas->removeControl(m_pControl.get());
            }
        }
    }
private:
    osg::ref_ptr<osgEarth::Controls::ControlCanvas> m_pCanvas;
    osg::ref_ptr<osgEarth::Controls::Control> m_pControl;
    bool                                      m_bAdd;
};

/**
 * @brief 给布局节点添加节点
 */
class CAddControl2Container:public osg::Operation
{
public:
    CAddControl2Container(osgEarth::Controls::Container* pContainer,
                          osgEarth::Controls::Control* pControl):
        m_pContainer(pContainer),
        m_pControl(pControl){}

    void operator()(osg::Object*)
    {
        if(m_pControl.valid()) m_pControl->setAlign(osgEarth::Controls::Control::ALIGN_NONE,osgEarth::Controls::Control::ALIGN_NONE);
        if(m_pContainer.valid()) m_pContainer->addControl(m_pControl.get());
    }
private:
    osg::ref_ptr<osgEarth::Controls::Container> m_pContainer;
    osg::ref_ptr<osgEarth::Controls::Control> m_pControl;
};

/**
 * @brief 修改界面布局节点
 */
class CAddControls2Container:public osg::Operation
{
public:
    CAddControls2Container(osgEarth::Controls::Container* pContainer
                           ,osgEarth::Controls::ControlVector pControl):
        m_pContainer(pContainer)
      ,m_pControl(pControl){}

    void operator()(osg::Object*)
    {
        if(m_pContainer.valid()) m_pContainer->addControls(m_pControl);
    }
private:
    osg::ref_ptr<osgEarth::Controls::Container> m_pContainer;
    osgEarth::Controls::ControlVector m_pControl;
};

/**
 * @brief 清空所有的子节点
 */
class CClearContainer:public osg::Operation
{
public:
    CClearContainer(osgEarth::Controls::Container* pContainer):
        m_pContainer(pContainer){}

    void operator()(osg::Object*)
    {
        if(m_pContainer.valid()) m_pContainer->clearControls();
    }
private:
    osg::ref_ptr<osgEarth::Controls::Container> m_pContainer;
};

/**
 * @brief 将节点绑定到指定节点的父节点上
 */
class COSGAttachNode:public osg::Operation
{
public:
    COSGAttachNode(osg::Node* pNode,osg::Node* pAttachNode,bool bAdd):
        m_pToAttachNode(pNode),m_pAttachNode(pAttachNode),m_bAdd(bAdd){}

    void operator()(osg::Object*)
    {
        if(m_pToAttachNode.valid())
        {
            auto parents = m_pToAttachNode->getParents();
            for(auto one : parents)
            {
                if(m_bAdd)
                {
                    one->addChild(m_pAttachNode);
                }
                else
                {
                    one->removeChild(m_pAttachNode);
                }
            }
        }
    }
private:
    osg::ref_ptr<osg::Node> m_pToAttachNode;
    osg::ref_ptr<osg::Node> m_pAttachNode;
    bool                    m_bAdd;
};

/**
 * @brief 给视图添加回调
 */
class CModifyViewHandler:public osg::Operation
{
public:
    CModifyViewHandler(osgViewer::View* pView,osgGA::GUIEventHandler* pEventHandler,bool bAdd):
        m_pView(pView),m_pEventHandler(pEventHandler),m_bAdd(bAdd){}

    void operator()(osg::Object*)
    {
        if(m_pView.valid())
        {
            if(m_bAdd)
            {
                m_pView->addEventHandler(m_pEventHandler);
            }
            else
            {
                m_pView->removeEventHandler(m_pEventHandler);
            }
        }
    }
private:
    osg::ref_ptr<osgViewer::View> m_pView;
    osg::ref_ptr<osgGA::GUIEventHandler> m_pEventHandler;
    bool                    m_bAdd;
};

#include <Inner/IRender.h>
class CRenderThreadCall:public osg::Operation
{
public:
    CRenderThreadCall(IRender* pRender):osg::Operation("Render",true),m_pRender(pRender){}

    void operator()(osg::Object*)
    {
        m_pRender->UpdateRender();
    }
private:
    IRender* m_pRender;
};

#endif // OSGEXTERN_HEADER_H
