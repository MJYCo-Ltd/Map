#ifndef INTERFACE_VIEW_TYPE_HEARDER_H
#define INTERFACE_VIEW_TYPE_HEARDER_H

#include <SceneGraph/SceneType.h>

const int C_WINDOW_HEIGHT(100);
const int C_WINDOW_WIDTH(100);

/// 视点位置
struct SceneViewPoint
{
    ScenePos stPos;
    float fAzimuth{};  /// 方位角 [deg][0~360]
    float fElev{};     /// 俯仰角 [deg][0~90]
    float fDistance{10.f};/// 距视点位置的距离[m]

    ///重写判等操作符
    bool operator == (const SceneViewPoint& rOther) const
    {
        if(&rOther == this)
        {
            return(true);
        }

        if(rOther.stPos == stPos &&
                fabs(rOther.fElev - fElev) < FLT_EPSILON &&
                fabs(rOther.fAzimuth - fAzimuth) < FLT_EPSILON &&
                fabs(rOther.fDistance-fDistance) < FLT_EPSILON)
        {
            return(true);
        }
        else
        {
            return(false);
        }
    }

    bool operator !=(const SceneViewPoint& rOther) const
    {
        return(!(this->operator==(rOther)));
    }
};

/// 视口位置
struct CameraViewPort
{
    int nX{};      /// 视口在屏幕X轴的位置
    int nY{};      /// 视口在屏幕Y轴的位置
    int nWidth{};  /// 视口的宽度
    int nHeight{}; /// 视口的高度

    /// 判断两个是否相等
    bool operator == (const CameraViewPort& rOther) const
    {
        if(&rOther == this)
        {
            return(true);
        }

        if(rOther.nX == nX &&
                rOther.nY == nY &&
                rOther.nWidth == nWidth &&
                rOther.nHeight == nHeight)
        {
            return(true);
        }
        else
        {
            return(false);
        }
    }

    /// 判断两者是否不相等
    bool operator != (const CameraViewPort& rOther) const
    {
        return(!this->operator==(rOther));
    }

    bool IsValid()const
    {
        return(0 != nWidth && 0 != nHeight);
    }
};

/// 视口类型
enum ProjectType
{
    Perspective, /// 透视投影
    Ortho        /// 正视投影
};

#endif /// INTERFACE_VIEW_TYPE_HEARDER_H
