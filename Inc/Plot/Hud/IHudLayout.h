#ifndef INTERFACE_HUD_LAYOUT_HEARDER_H
#define INTERFACE_HUD_LAYOUT_HEARDER_H
#include <Hud/IHudNode.h>

/**
 * @brief 视口的屏显类
 */
class IHudLayout:public IHudNode
{
public:
    CONSTRUCTOR(IHudLayout,IHudNode)
    enum LAYOUT_TYPE
    {
        LAYOUT_VERTICAL,  /// 垂直布局
        LAYOUT_HORIZONTAL /// 水平布局
    };

    /**
     * @brief 设置布局类型
     */
    void SetLayoutType(LAYOUT_TYPE type)JUDGE_EQUAL_CALL_FUNCTION(type,m_emType,LayoutTypeChanged)
    LAYOUT_TYPE LayoutType()const{return(m_emType);}

    /**
     * @brief 增加或者移除节点
     */
    virtual bool AddHudNode(IHudNode* pNode)=0;
    virtual bool RemoveHudNode(IHudNode* pNode)=0;

protected:
    virtual ~IHudLayout(){}
    virtual void LayoutTypeChanged()=0;
protected:
    LAYOUT_TYPE m_emType{LAYOUT_HORIZONTAL};
};

#endif// INTERFACE_HUD_LAYOUT_HEARDER_H
