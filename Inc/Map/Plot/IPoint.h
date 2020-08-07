#ifndef INTERFACE_POINT_HEARDER_H
#define INTERFACE_POINT_HEARDER_H
#include "IDrawShape.h"

/**
 * @brief 场景节点类
 */
class IPoint:public IDrawShape
{
public:
    /**
     * @brief 设置点的大小
     */
    virtual void SetPointSize(float)=0;
    virtual const float& GetPointSize()=0;

    /**
     * @brief 设置点的大小
     */
    virtual void SetColor(const SceneColor&)=0;
    virtual const SceneColor& GetColor()=0;
};

#endif
