#ifndef H_SCENE_TYPE_INCLUDE_H
#define H_SCENE_TYPE_INCLUDE_H
#include <cmath>

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
    float fR=0.0f;
    float fG=0.0f;
    float fB=0.0f;
    float fA=1.0f;

    bool operator == (const SceneColor& rOther) const
    {
        if(&rOther == this)
        {
            return(true);
        }

        if(fabs(fR - rOther.fR) < 1e-6
         &&fabs(fG - rOther.fG) < 1e-6
         &&fabs(fB - rOther.fB) < 1e-6
         &&fabs(fA - rOther.fA) < 1e-6)
        {
            return(true);
        }
        else
        {
            return(false);
        }
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

        if(fabs(dYaw - rOther.dYaw) < 1e-6
         &&fabs(dPitch - rOther.dPitch) < 1e-6
         &&fabs(dRoll - rOther.dRoll) < 1e-6
         && rOther.rotaOrder == rotaOrder)
        {
            return(true);
        }
        else
        {
            return(false);
        }
    }

    bool operator !=(const SceneAttitude& rOther) const
    {
        return(!(this->operator==(rOther)));
    }
};

/**
 * @brief 视点位置
 */
union ScenePos
{
    struct
    {
        float fLon;    /// 经度 [度]
        float fLat;    /// 纬度 [度]
        float fHeight; /// 高度 [米]
        bool  bIsGeo;  /// 是否是经纬度
    };

    struct
    {
        float fX;    /// x轴坐标
        float fY;    /// y轴坐标
        float fZ;    /// z轴坐标
    };

    bool operator == (const ScenePos& rOther) const
    {
        if(&rOther == this)
        {
            return(true);
        }

        if(fabs(fLon - rOther.fLon) < 1e-6
         &&fabs(fLat - rOther.fLat) < 1e-6
         &&fabs(fHeight - rOther.fHeight) < 1e-6
         && bIsGeo == rOther.bIsGeo)
        {
            return(true);
        }
        else
        {
            return(false);
        }
    }

    bool operator !=(const ScenePos& rOther) const
    {
        return(!(this->operator==(rOther)));
    }
};

#endif
