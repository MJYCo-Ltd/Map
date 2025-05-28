#ifndef QT_VIEWHUD_H
#define QT_VIEWHUD_H
#include <set>
#include <osg/View>
#include <osgEarth/Controls>
#include <Hud/IViewHud.h>
#include <Inner/ImplSceneNode.hpp>

class QtViewHud:public ImplSceneNode<IViewHud>
{
public:
    QtViewHud(osg::View*,ISceneGraph*pSceneGraph);
    ~QtViewHud();

    /**
     * @brief 添加屏显节点
     */
    virtual bool AddHudNode(IHudNode* pHudNode);

    /**
     * @brief 移除屏显节点
     */
    virtual bool RemoveHudNode(IHudNode* pHudNode);

    /**
     * @brief 清空所有的节点
     */
    virtual void Clear();
    /**
     * @brief 当前是否有选中hud节点
     */
    virtual bool CurrentHaveSelect()
    {
        return m_CurrentHaveSlect;
    }
    /**
     * @brief 当前是否有选中hud节点
     */
    virtual void SetCurrentHaveSelect(bool valb)
    {
        m_CurrentHaveSlect = valb;
    }

protected:
    void InitNode();
    void AddControl(osgEarth::Controls::ControlCanvas* pCanvas,osgEarth::Controls::Control* pControl);
    void DelControl(osgEarth::Controls::ControlCanvas* pCanvas,osgEarth::Controls::Control* pControl);
private:
    osg::observer_ptr<osg::View> m_pView;
    osg::observer_ptr<osgEarth::Controls::ControlCanvas> m_pControlCanvas;
    std::set<IHudNode*> m_setHudNode;
    bool m_CurrentHaveSlect = false;
};

#endif // QTVIEWHUD_H
