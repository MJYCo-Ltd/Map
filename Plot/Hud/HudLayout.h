#ifndef CHUDLAYOUT_H
#define CHUDLAYOUT_H
#include <list>
#include <Plot/Hud/IHudLayout.h>
#include <Inner/ImplHudNode.hpp>

class CHudLayout:public ImplHudNode<IHudLayout>
{
    friend class CVBox;
public:
    CONSTRUCTOR(CHudLayout,ImplHudNode<IHudLayout>)
    ~CHudLayout()
    {
        if(m_pScenePos)
        {
            delete m_pScenePos;
        }
        if(m_pMoveScenePos)
        {
            delete m_pMoveScenePos;
        }
    }
    /**
     * @brief 管理节点
     */
    bool AddHudNode(IHudNode* pNode);
    bool RemoveHudNode(IHudNode* pNode);
    void SetModelScenePos(ScenePos *sp)
    {
        if(!m_pScenePos){
            m_pScenePos = new ScenePos;
        }
        *m_pScenePos = *sp;
    }
    ScenePos * ModelScenePos(){return m_pScenePos;}
    void SetMoveModelScenePos(ScenePos *sp)
    {
        if(!m_pMoveScenePos){
            m_pMoveScenePos = new ScenePos;
        }
        *m_pMoveScenePos = *sp;
    }
    ScenePos * MoveModelScenePos(){return m_pMoveScenePos;}
    /**
     * @brief 管理观察者
     */
    bool SubObserver(const IHudLayoutMouseObserver* pObserver);
    bool UnSubObserver(const IHudLayoutMouseObserver* pObserver);
    virtual std::list<IHudNode*> GetAllChildrenNode();

protected:
    void InitNode();
    void FrameCall();
    void LayoutTypeChanged()SET_TRUE_NODE_UPDATE(m_bLayoutChanged)
    void ColorChanged()SET_TRUE_NODE_UPDATE(m_bColorChanged)
    void OutLineColorChanged()SET_TRUE_NODE_UPDATE(m_bOutLineColorChanged)

protected:
    bool m_bLayoutChanged{false};
    bool m_bColorChanged{false};
    bool m_bOutLineColorChanged{false};
    osg::observer_ptr<osgEarth::Controls::Container> m_pLayout;
    osg::Node* m_line = nullptr;
    std::list<IOsgSceneNode*> m_pAllChild;
    std::list<const IHudLayoutMouseObserver*> m_pAllObserver;
    ScenePos *m_pScenePos = nullptr;
    ScenePos *m_pMoveScenePos = nullptr;
};

#endif // CHUDLAYOUT_H
