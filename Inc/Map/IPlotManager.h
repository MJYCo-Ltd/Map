#ifndef INTERFACE_PLOTMANGER_HEADER_H
#define INTERFACE_PLOTMANGER_HEADER_H

#include <string>

using namespace std;

class IPlotLayer;
class IMapSceneNode;

/**
 * @brief 标绘相关的消息
 */
class IPlotMessage
{
public:
    virtual ~IPlotMessage(){}
};

/**
 * @brief 标绘管理类
 */
class IPlotManager
{
public:
    virtual ~IPlotManager(){}

    /**
     * @brief 创建标绘图层
     * @return
     */
    virtual IPlotLayer* FindOrCreateLayer(const string&)=0;

    /**
     * @brief 移除图层
     * @return
     */
    virtual bool RemoveLayer(IPlotLayer*&)=0;

    /**
     * @brief 移除图层
     * @return
     */
    virtual bool RemoveLayer(const string&)=0;

    /**
     * @brief 清空所有图层
     */
    virtual void Clear()=0;

    /**
     * @brief 根据字符串创建一个地图节点
     * @return
     */
    virtual IMapSceneNode* CreateMapSceneNode(const string&)=0;

    /**
     * @brief 删除地图节点
     * @return
     */
    virtual bool DeleteMapSceneNode(IMapSceneNode*)=0;
};

#endif
