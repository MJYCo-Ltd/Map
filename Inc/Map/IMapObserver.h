#ifndef INTERFACE_MAP_OBSERVER_HEARDER_H
#define INTERFACE_MAP_OBSERVER_HEARDER_H

#include <string>
#include <Map/MapType.h>

/// 地图消息观察者
struct IMapMessageObserver
{
    /**
     * @brief 地图类型更改
     *        地图都进行修改
     */
    virtual void MapTypeChanged(MapType){}

    /**
     * @brief 图层更改消息
     */
    virtual void AddLayer(const std::string&){}
    virtual void RemoveLayer(const std::string&){}
};

#endif /// INTERFACE_MAP_OBSERVER_HEARDER_H
