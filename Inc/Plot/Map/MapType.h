#ifndef INTERFACE_MAP_TYPE_HEADER_H
#define INTERFACE_MAP_TYPE_HEADER_H
#include <vector>
#include <cmath>
#include <limits>

/**
 * @brief 地图经纬度位置
 */
struct MapGeoPos
{
    float fLon=0.f;
    float fLat=0.f;
    float fHeight=0.f;

    bool operator == (const MapGeoPos& rOther) const
    {
        if(&rOther == this)
        {
            return(true);
        }

        return(fabs(fLon - rOther.fLon) < FLT_EPSILON
          &&fabs(fLat - rOther.fLat) < FLT_EPSILON
          &&fabs(fHeight - rOther.fHeight) < FLT_EPSILON);
    }

    bool operator !=(const MapGeoPos& rOther) const
    {
        return(!(this->operator==(rOther)));
    }
};

/**
 * @brief 地图多边形
 */
struct MapPolygon
{
    std::vector<MapGeoPos> vAllGeoPos;

    bool operator == (const MapPolygon& rOther) const
    {
        if(&rOther == this)
        {
            return(true);
        }

        return(rOther.vAllGeoPos == vAllGeoPos);
    }

    bool operator !=(const MapPolygon& rOther) const
    {
        return(!(this->operator==(rOther)));
    }
};

/**
 * @brief 地图节点高程格式
 */
enum MAP_TERRAIN
{
    CLOSE_TERRAIN,    /// 贴地
    RELATIVE_TERRAIN, /// 相对高程高度
    ABSOLUTE_TERRAIN  /// 相对椭球表面高度
};

enum MapType
{
    MAP_3D,   /// 三维地图
    MAP_2D    /// 二维地图
};

#endif