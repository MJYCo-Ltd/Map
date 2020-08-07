#ifndef H_SCENE_TYPE_INCLUDE_H
#define H_SCENE_TYPE_INCLUDE_H
#include <cmath>

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
