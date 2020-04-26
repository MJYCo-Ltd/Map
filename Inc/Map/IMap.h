#ifndef INTERFACE_MAP_HEARDER_H
#define INTERFACE_MAP_HEARDER_H

#include <list>
#include <string>
#include "../SceneGraph/ISceneNode.h"

using namespace std;

/**
 * @brief 如果图层名称是中文
 *        将采用 utf-8编码
 */
typedef list<string> MapLayers;

enum MapType
{
    MAP_3D,   /// 默认加载三维地图场景
    MAP_2D    /// 默认加载二维地图场景
};

class IPlotManager;
class ISpaceEnv;

struct IMapMessageObserver
{
    /**
     * @brief 地图类型更改
     *        地图都进行修改
     */
    virtual void MapTypeChanged(MapType){}
};

/**
 * @brief 场景节点类
 */
class IMap:public ISceneNode
{
public:

    /**
     * @brief 注册消息
     */
    virtual void SubMessage(IMapMessageObserver* pMsgObr)=0;

    /**
     * @brief 取消注册消息
     */
    virtual void UnSubMessage(IMapMessageObserver* pMsgObr)=0;

    /**
     * @brief 坐标转换
     * @param 屏幕x轴坐标
     * @param 屏幕y轴坐标
     * @param 地理坐标 [度 度 米]
     * @param 转换类型 0表示从屏幕坐标转换成地理坐标，1表示由地理坐标转换成屏幕坐标
     * @return
     */
    virtual bool ConvertCoord(int&, int&, ScenePos&, short)=0;

    /**
     * @brief 获取地图图层
     * @return
     */
    virtual const MapLayers& GetMapLayers()=0;

    /**
     * @brief 控制地图图层显隐
     * @param  要控制的图层名称
     */
    virtual void SetLayerVisible(const string&)=0;

    /**
     * @brief 更改地图类型
     */
    virtual void ChangeMapType(MapType)=0;

    /**
     * @brief  获取标绘管理类
     * @return
     */
    virtual IPlotManager* GetPlotManager()=0;

    /**
     * @brief 获取星空管理类
     * @return
     */
    virtual ISpaceEnv* GetSpaceEnv()=0;
};

#endif
