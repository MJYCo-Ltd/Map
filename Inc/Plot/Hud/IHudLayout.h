#ifndef INTERFACE_HUD_LAYOUT_HEARDER_H
#define INTERFACE_HUD_LAYOUT_HEARDER_H
#include <Hud/IHudNode.h>
#include <list>
class IHudLayout;

class IHudLayoutMouseObserver
{
public:
    virtual ~IHudLayoutMouseObserver(){}
    virtual void DoubleClick(const IHudLayout*) const=0;
    virtual void Move(const IHudLayout*,const HudPos&,int nWidth, int nHeight)const=0;
};

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
     * @brief 设置字体填充颜色
     */
    void SetFillColor(const SceneColor& rColor)JUDGE_EQUAL_CALL_FUNCTION(rColor,m_stColor,ColorChanged)
    const SceneColor& FillColor()const{return(m_stColor);}

    /**
     * @brief 设置字体填充颜色
     */
    void SetOutLineColor(const SceneColor& rColor)JUDGE_EQUAL_CALL_FUNCTION(rColor,m_stOutLineColor,OutLineColorChanged)
    const SceneColor& OutLineColor()const{return(m_stOutLineColor);}

    /**
     * @brief 增加或者移除节点
     */
    virtual bool AddHudNode(IHudNode* pNode)=0;
    virtual bool RemoveHudNode(IHudNode* pNode)=0;

    /**
     *
     */
    virtual std::list<IHudNode*> GetAllChildrenNode(){return std::list<IHudNode*>();}
    /**
     * @brief 设置模型位置
     */
    virtual void SetModelScenePos(ScenePos *sp)=0;
    virtual ScenePos * ModelScenePos()=0;
    /**
     * @brief 增加或者移除节点
     */
    virtual void SetMoveModelScenePos(ScenePos *sp)=0;
    virtual ScenePos * MoveModelScenePos()=0;

    /**
     * @brief 订阅或取消观察者
     */
    virtual bool SubObserver(const IHudLayoutMouseObserver* pObserver)=0;
    virtual bool UnSubObserver(const IHudLayoutMouseObserver* pObserver)=0;

protected:
    virtual ~IHudLayout(){}
    virtual void LayoutTypeChanged()=0;
    virtual void ColorChanged()=0;
    virtual void OutLineColorChanged()=0;
protected:
    LAYOUT_TYPE m_emType{LAYOUT_HORIZONTAL};
    SceneColor  m_stColor;
    SceneColor  m_stOutLineColor;
};

#endif// INTERFACE_HUD_LAYOUT_HEARDER_H
