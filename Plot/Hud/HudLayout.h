#ifndef CHUDLAYOUT_H
#define CHUDLAYOUT_H
#include <list>
#include <Plot/Hud/IHudLayout.h>
#include <Inner/ImplHudNode.hpp>

class CHudLayout:public ImplHudNode<IHudLayout>
{
public:
    CONSTRUCTOR(CHudLayout,ImplHudNode<IHudLayout>)

    /**
     * @brief 管理节点
     */
    bool AddHudNode(IHudNode* pNode);
    bool RemoveHudNode(IHudNode* pNode);

protected:
    void InitNode();
    void FrameCall();
    void LayoutTypeChanged()SET_TRUE_NODE_UPDATE(m_bLayoutChanged)

protected:
    bool m_bLayoutChanged{false};
    osg::observer_ptr<osgEarth::Controls::Container> m_pLayout;
    std::list<IOsgSceneNode*> m_pAllChild;
};

#endif // CHUDLAYOUT_H
