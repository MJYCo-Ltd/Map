#ifndef INTERFACE_SENSOR_HEARDER_H
#define INTERFACE_SENSOR_HEARDER_H
#include "../IMapSceneNode.h"

/**
 * @brief 传感器基类
 */
class ISensor:public IMapSceneNode
{
public:
    virtual ~ISensor(){}

    /**
     * @brief 设置颜色
     */
    virtual void SetColor(const SceneColor&)=0;
    virtual const SceneColor& Color() const=0;

    /**
     * @brief 设置探测距离
     */
    virtual void SetDistance(double)=0;
    virtual double Distance()const=0;

};

#endif
