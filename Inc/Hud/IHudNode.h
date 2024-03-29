#ifndef INTERFACE_HUD_NODE_HEARDER_H
#define INTERFACE_HUD_NODE_HEARDER_H
#include <SceneGraph/ISceneNode.h>
#include <Hud/HudType.h>
/**
 * @brief 视口的屏显类
 */
class IHudNode:public ISceneNode
{
public:
    CONSTRUCTOR(IHudNode,ISceneNode)

    /**
     * @brief 设置节点在屏幕上的位置
     * @return
     */
    void SetHudPos(const HudPos& rHudPos)JUDGE_EQUAL_CALL_FUNCTION(rHudPos,m_stHudPos,HudPosChanged)
    const HudPos& GetHudPos(){return(m_stHudPos);}

    /**
     * @brief 设置节点在屏幕位置的类型
     * @return
     */
    void SetPosType(HUD_POS_TYPE type)JUDGE_EQUAL_CALL_FUNCTION(type,m_emHudPosType,HudTypeChanged)
    const HUD_POS_TYPE PosType()const{return(m_emHudPosType);}

    virtual void HudTypeChanged()=0;
protected:
    virtual ~IHudNode(){}
    virtual void HudPosChanged()=0;

protected:
    HudPos       m_stHudPos;
    HUD_POS_TYPE m_emHudPosType{HUD_DOWN_LEFT};
};

#endif /// INTERFACE_HUD_NODE_HEARDER_H
