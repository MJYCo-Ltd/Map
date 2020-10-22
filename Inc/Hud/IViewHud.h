#ifndef INTERFACE_VIEW_HUD_HEARDER_H
#define INTERFACE_VIEW_HUD_HEARDER_H

#include <Hud/IHudNode.h>

/**
 * @brief 视口的屏显类
 */
class IViewHud
{
public:
    CONSTRUCTOR(IViewHud,m_pSceneGraph)

    /**
     * @brief 添加屏显节点
     */
    virtual void AddHudNode(IHudNode* pHudNode)=0;

    /**
     * @brief 移除屏显节点
     */
    virtual void RemoveHudNode(IHudNode* pHudNode)=0;

    /**
     * @brief 清空所有的节点
     */
    virtual void Clear()=0;
protected:
    virtual ~IViewHud(){}
protected:
    ISceneGraph* m_pSceneGraph;
};

#endif
