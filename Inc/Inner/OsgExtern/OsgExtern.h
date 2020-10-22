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
    void operator()(osg::Object*){m_pParent->removeChildren(0,m_pParent->getNumChildren());}
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
        if(m_bAdd)
        {
            m_pParent->addChild(m_pChild);
        }
        else
        {
            m_pParent->removeChild(m_pChild);
        }
    }
private:
    osg::ref_ptr<osg::Group> m_pParent; /// 父节点
    osg::ref_ptr<osg::Node>  m_pChild;  /// 子节点
    bool                     m_bAdd;    /// 是否增加
};

class CModifyControl:public osg::Operation
{
public:
    CModifyControl(osgEarth::Controls::ControlCanvas* pCanvas
                   ,osgEarth::Controls::Control*pControl,bool bAdd):
        m_pCanvas(pCanvas),m_pControl(pControl),m_bAdd(bAdd){}

    void operator()(osg::Object*)
    {
        if(m_bAdd)
        {
            m_pCanvas->addControl(m_pControl.get());
        }
        else
        {
            m_pCanvas->removeControl(m_pControl);
        }
    }
private:
    osg::ref_ptr<osgEarth::Controls::ControlCanvas> m_pCanvas;
    osg::ref_ptr<osgEarth::Controls::Control> m_pControl;
    bool                                      m_bAdd;
};

#endif // MAP_GLOBAL_H
