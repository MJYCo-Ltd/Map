#ifndef INTERFACE_FLASH_ABILITY_HEARDER_H
#define INTERFACE_FLASH_ABILITY_HEARDER_H

#include <SceneGraph/SceneType.h>
#include <SceneGraph/INodeAbility.h>

/**
 * @brief 场景节点类
 */
class IFlashAbility:public INodeAbility
{
public:

    /**
     * @brief 转换成本类型
     * @return
     */
    IFlashAbility* AsFlashAbility(){return(this);}

    /**
     * @brief 设置闪烁颜色
     * @param rColor
     */
    void SetFlashColor(const SceneColor& rColor)JUDGE_EQUAL_CALL_FUNCTION(rColor,m_stFlahColor,FlashColorChanged)
    const SceneColor& GetFlashColor()const{return(m_stFlahColor);}

    /**
     * @brief 设置闪烁频率
     * @param nHZ > 60 将不再增加 nHZ <=0 将保持FlashColor不变即实现变色功能
     */
    void SetFlashFreq(int nHZ)JUDGE_EQUAL_CALL_FUNCTION(nHZ,m_nFlashHZ,FlashStatusChanged)
    int  GetFlashFreq(){return(m_nFlashHZ);}

    /**
     * @brief 是否开始闪烁
     */
    void SetFlash(bool bFlash)JUDGE_EQUAL_CALL_FUNCTION(bFlash,m_bFlash,FlashFlashChanged)
    bool IsFlash(){return(m_bFlash);}
protected:
    virtual ~IFlashAbility(){}

    virtual void FlashStatusChanged()=0;
    virtual void FlashColorChanged()=0;
    virtual void FlashFlashChanged()=0;
protected:
    bool         m_bFlash{true};
    SceneColor   m_stFlahColor;
    int          m_nFlashHZ{1};
};

#endif //INTERFACE_FLASH_ABILITY_HEARDER_H
