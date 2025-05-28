#ifndef INTERFACE_VIEW_HUD_HEARDER_H
#define INTERFACE_VIEW_HUD_HEARDER_H

#include <Hud/IHudNode.h>
/**
 * @brief 视口的屏显类
 */
class IViewHud:public ISceneNode
{
public:
    CONSTRUCTOR(IViewHud,ISceneNode)

    /**
     * @brief 添加屏显节点
     */
    virtual bool AddHudNode(IHudNode* pHudNode)=0;

    /**
     * @brief 移除屏显节点
     */
    virtual bool RemoveHudNode(IHudNode* pHudNode)=0;

    /**
     * @brief 清空所有的节点
     */
    virtual void Clear()=0;
    /**
     * @brief 当前是否有选中hud节点
     */
    virtual bool CurrentHaveSelect()=0;
    /**
     * @brief 当前是否有选中hud节点
     */
    virtual void SetCurrentHaveSelect(bool valb)=0;
protected:
    virtual ~IViewHud(){}
};

#endif
