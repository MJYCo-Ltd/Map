#ifndef INTERFACE_VIEW_HUD_HEARDER_H
#define INTERFACE_VIEW_HUD_HEARDER_H

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
 * @brief 视口的屏显类
 */
class IViewHud
{
public:
protected:
    virtual ~IViewHud(){}
};

#endif
