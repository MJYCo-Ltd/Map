#ifndef INTERFACE_MAP_OBSERVER_HEARDER_H
#define INTERFACE_MAP_OBSERVER_HEARDER_H

#include <string>
#include <Plot/Map/MapType.h>
#include <SceneGraph/IWindow.h>
/// 地图消息观察者
struct IMapMessageObserver
{
    /**
     * @brief 地图类型更改
     * @param 地图类型
     */
    virtual void MapTypeChanged(MapType){}

    /**
     * @brief 图层增加消息
     * @param 增加的图层的名字 [utf-8编码]
     */
    virtual void AddLayer(const std::string&){}

    /**
     * @brief 图层移除消息
     * @param 移除的图层的名字 [utf-8编码]
     */
    virtual void RemoveLayer(const std::string&){}

    /**
     * @brief 鼠标所在的经纬度信息
     * @param 第1个参数 经度信息 [deg](-180,180)
     * @param 第2个参数 纬度信息 [deg][-90,90]
     * @param 第3个参数 高度信息 [m] 相对于wgs84椭球表面的高度
     */
    virtual void MousePos(double,double,double){}

    virtual void MouseDown(MouseButtonMask type, double,double,double){};

    virtual void MouseDblClick(MouseButtonMask type, double,double,double){};
};

#endif /// INTERFACE_MAP_OBSERVER_HEARDER_H
