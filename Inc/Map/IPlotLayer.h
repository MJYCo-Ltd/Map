#ifndef INTERFACE_PLOTLAYER_HEADER_H
#define INTERFACE_PLOTLAYER_HEADER_H
#include<string>
class IMapSceneNode;

using namespace std;

struct IPlotLayer
{
    virtual ~IPlotLayer(){}

    /**
     * @brief 获取图层名字
     * @return
     */
    virtual const string& GetLayerName()=0;

    /**
     * @brief 添加节点
     */
    virtual unsigned int AddSceneNode(IMapSceneNode*)=0;

    /**
     * @brief 移除节点
     */
    virtual void RemoveSceneNode(IMapSceneNode*)=0;

    /**
     * @brief 清空所有的节点
     */
    virtual void Clear()=0;

    /**
     * @brief 控制图层显隐
     */
    virtual void SetVisible(bool)=0;

    /**
     * @brief 图层是否可见
     * @return
     */
    virtual bool IsVisible()=0;
};

#endif
