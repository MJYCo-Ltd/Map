#ifndef H_SCENE_TYPE_INCLUDE_H
#define H_SCENE_TYPE_INCLUDE_H

#include <cmath>
#include <limits>

/**
 * @brief 俯仰翻滚偏航的旋转类型
 */
enum Scene_ROTATE
{
    SR_XYZ,SR_XZY,SR_YXZ,SR_YZX,SR_ZXY,SR_ZYX /// 上下两种方式相同
   ,SR_RPY,SR_RYP,SR_PRY,SR_PYR,SR_YRP,SR_YPR
};

/// 场景颜色
struct SceneColor
{
    float fR=0.f;
    float fG=0.f;
    float fB=0.f;
    float fA=1.f;

    bool operator == (const SceneColor& rOther) const
    {
        if(&rOther == this)
        {
            return(true);
        }

        return(fabs(fR - rOther.fR) < FLT_EPSILON
         &&fabs(fG - rOther.fG) < FLT_EPSILON
         &&fabs(fB - rOther.fB) < FLT_EPSILON
         &&fabs(fA - rOther.fA) < FLT_EPSILON);
    }

    bool operator !=(const SceneColor& rOther) const
    {
        return(!(this->operator==(rOther)));
    }
};

/// 姿态信息
struct SceneAttitude
{
    Scene_ROTATE rotaOrder=SR_YPR; /// 旋转顺序
    double dYaw=0.;    /// 方位 [0~360)  绕Z轴旋转
    double dPitch=0.;  /// 俯仰 [-90,90] 绕Y轴旋转
    double dRoll=0.;   /// 翻滚 [0~360)  绕X轴旋转

    bool operator == (const SceneAttitude& rOther) const
    {
        if(&rOther == this)
        {
            return(true);
        }

        return(rOther.rotaOrder == rotaOrder
         && fabs(dYaw - rOther.dYaw) < DBL_EPSILON
         && fabs(dPitch - rOther.dPitch) < DBL_EPSILON
         && fabs(dRoll - rOther.dRoll) < DBL_EPSILON);
    }

    bool operator !=(const SceneAttitude& rOther) const
    {
        return(!(this->operator==(rOther)));
    }
};

/**
 * @brief 视点位置
 */
struct ScenePos
{
    float fX=0.f;    /// x轴坐标
    float fY=0.f;    /// y轴坐标
    float fZ=0.f;    /// z轴坐标

    bool operator == (const ScenePos& rOther) const
    {
        if(&rOther == this)
        {
            return(true);
        }

        return(fabs(fX - rOther.fX) < FLT_EPSILON
        &&fabs(fY - rOther.fY) < FLT_EPSILON
        &&fabs(fZ - rOther.fZ) < FLT_EPSILON);
    }

    bool operator !=(const ScenePos& rOther) const
    {
        return(!(this->operator==(rOther)));
    }
};

#endif
