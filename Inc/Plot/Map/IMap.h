#ifndef INTERFACE_MAP_HEARDER_H
#define INTERFACE_MAP_HEARDER_H

#include <list>
#include <string>
#include <SceneGraph/ISceneGroup.h>
#include <Plot/Map/MapType.h>

/**
 * @brief 如果图层名称是中文
 *        将采用 utf-8编码
 */
typedef std::list<std::string> MapLayers;


class ISpaceEnv;
struct IMapMessageObserver;
class IMapLayer;

/**
 * @brief 地图节点
 */
class IMap:public ISceneGroup
{
public:
    CONSTRUCTOR(IMap,ISceneGroup)

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
    virtual bool ConvertCoord(float&, float&, MapGeoPos&, short)=0;

    /**
     * @brief 获取地图图层
     * @return
     */
    virtual MapLayers GetMapLayers() const =0;

    /**
     * @brief 控制地图图层显隐
     * @param  要控制的图层名称
     */
    virtual void SetLayerVisible(const std::string&)=0;

    /**
     * @brief 创建一个图层
     * @return
     */
    virtual IMapLayer* CreateLayer(const std::string&)=0;

    /**
     * @brief 移除图层
     * @return
     */
    virtual bool RemoveLayer(IMapLayer*&)=0;
    virtual bool RemoveLayer(const std::string& sLayerName)=0;
    /**
     * @brief 清空所有的图层
     */
    virtual void ClearLayers()=0;

    /**
     * @brief 更改地图类型
     */
    virtual void ChangeMapType(MapType)=0;

    /**
     * @brief 获取地惯系节点
     * @return
     */
    virtual ISpaceEnv* GetSpaceEnv()=0;

    /**
     * @brief 开启/关闭 地球自转
     * @attention 默认开启
     */
    virtual void SetEarthSelfRotate(bool)=0;

    /**
     * @brief 更新时间
     * @arg   约简儒略日
     */
    void UpdateDate(double dMJD)JUDGE_DOUBLE_CALL_FUNCTION(dMJD,m_dMJD,DateChanged)
protected:
    virtual ~IMap(){}
    virtual void DateChanged()=0;
protected:
    double m_dMJD=0.;
};

#endif
