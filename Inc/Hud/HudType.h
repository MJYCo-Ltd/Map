#ifndef INTERFACE_HUD_TYPE_HEARDER_H
#define INTERFACE_HUD_TYPE_HEARDER_H

enum HUD_POS_TYPE
{
    HUD_FIXED,     /// 固定位置
    HUD_UP_CENTER, /// 上部居中
    HUD_UP_RIGHT,  /// 右上角
    HUD_DOWN_LEFT, /// 左下角
    HUD_DOWN_RIGHT,/// 右下角
    HUD_DOWN_CENTER/// 下部居中
};

/**
 * @brief 屏幕坐标
 */
struct HudPos
{
    unsigned short nX=0; /// 屏幕X轴坐标
    unsigned short nY=0; /// 屏幕Y轴坐标

    bool operator == (const HudPos& rOther)const
    {
        if(&rOther == this)
        {
            return(true);
        }

        return(nX == rOther.nX && nY == rOther.nY);
    }

    bool operator!=(const HudPos& rOther)const
    {
        return(!(this->operator==(rOther)));
    }
};

#endif///INTERFACE_HUD_TYPE_HEARDER_H
