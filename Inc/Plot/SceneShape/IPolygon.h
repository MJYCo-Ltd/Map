#ifndef INTERFACE_POLYGON_HEARDER_H
#define INTERFACE_POLYGON_HEARDER_H
#include <vector>
#include <Plot/SceneShape/IGeometry.h>

/**
 * @brief 多边形节点
 * @attention 多边形的边不能自相交
 */
class IPolygon:public IGeometry
{
public:
    CONSTRUCTOR(IPolygon,IGeometry)

protected:
    virtual ~IPolygon(){}
};

#endif//INTERFACE_POLYGON_HEARDER_H
