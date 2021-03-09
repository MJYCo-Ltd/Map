#ifndef INTERFACE_SCENE_FLASH_GROUP_HEARDER_H
#define INTERFACE_SCENE_FLASH_GROUP_HEARDER_H

#include <SceneGraph/ISceneGroup.h>

/**
 * @brief 场景节点类
 */
class ISceneFlashGroup:public ISceneGroup
{
public:
    CONSTRUCTOR(ISceneFlashGroup,ISceneGroup)

    /**
     * @brief 转换成本类型
     * @return
     */
    ISceneFlashGroup* AsSceneFlashGroup(){return(this);}

    /**
     * @brief 设置闪烁颜色
     * @param rColor
     */
    void SetFlashColor(const SceneColor& rColor)JUDGE_EQUAL_CALL_FUNCTION(rColor,m_stFlahColor,FlashColorChanged)
    const SceneColor& GetFlashColor()const{return(m_stFlahColor);}

    /**
     * @brief 设置闪烁频率
     * @param nHZ
     */
    void SetFlashFreq(float fHZ){if(fHZ > 0 && fHZ < 30 && fabs(fHZ - m_fFlashHZ)>1e-7){m_fFlashHZ=fHZ;FlashStatusChanged();}}
    float  GetFlashFreq(){return(m_fFlashHZ);}

    /**
     * @brief 是否开始闪烁
     */
    void SetFlash(bool bFlash)JUDGE_EQUAL_CALL_FUNCTION(bFlash,m_bFlash,FlashFlashChanged)
    bool IsFlash(){return(m_bFlash);}
protected:
    virtual ~ISceneFlashGroup(){}

    virtual void FlashStatusChanged()=0;
    virtual void FlashColorChanged()=0;
    virtual void FlashFlashChanged()=0;
protected:
    bool         m_bFlash=true;
    SceneColor   m_stFlahColor;
    float        m_fFlashHZ=1.f;
};

#endif //INTERFACE_SCENE_FLASH_GROUP_HEARDER_H
