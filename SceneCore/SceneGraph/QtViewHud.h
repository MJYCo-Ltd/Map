#ifndef QT_VIEWHUD_H
#define QT_VIEWHUD_H
#include <osg/View>
#include <osgEarth/Controls>
#include <Hud/IViewHud.h>

class QtViewHud:public IViewHud
{
public:
    QtViewHud(osg::View*,ISceneGraph*pSceneGraph);
    ~QtViewHud();

    /**
     * @brief 添加屏显节点
     */
    virtual void AddHudNode(IHudNode* pHudNode);

    /**
     * @brief 移除屏显节点
     */
    virtual void RemoveHudNode(IHudNode* pHudNode);

    /**
     * @brief 清空所有的节点
     */
    virtual void Clear();
private:
    osg::observer_ptr<osg::View> m_pView;
    osg::ref_ptr<osgEarth::Controls::ControlCanvas> m_pControlCanvas;
};

#endif // QTVIEWHUD_H
