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

    /**
     * @brief 增加点
     */
    virtual void AddPoint(int,const ScenePos&)=0;

    /**
     * @brief 移除点
     */
    virtual bool RemovePoint(int)=0;

    /**
     * @brief 更新指定位置点
     */
    virtual bool UpdatePoint(int,const ScenePos&)=0;

    /**
     * @brief 设置多个位置
     */
    virtual void SetMultPos(const std::vector<ScenePos>&)=0;
    virtual std::vector<ScenePos> GetMulPos()const=0;

protected:
    virtual ~IPolygon(){}
};

#endif//INTERFACE_POLYGON_HEARDER_H
