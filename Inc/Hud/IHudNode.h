#ifndef INTERFACE_HUD_NODE_HEARDER_H
#define INTERFACE_HUD_NODE_HEARDER_H
#include <SceneGraph/SceneType.h>
#include <Hud/HudType.h>

class ISceneGraph;
/**
 * @brief 视口的屏显类
 */
class IHudNode
{
public:
    CONSTRUCTOR(IHudNode,m_pSceneGraph)

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
protected:
    virtual ~IHudNode(){}

    virtual void HudPosChanged()=0;
    virtual void HudTypeChanged()=0;

protected:
    HudPos       m_stHudPos;
    HUD_POS_TYPE m_emHudPosType=HUD_FIXED;
    ISceneGraph* m_pSceneGraph=nullptr;
};

#endif /// INTERFACE_HUD_NODE_HEARDER_H
