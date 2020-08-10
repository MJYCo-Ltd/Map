#ifndef INTERFACE_POLYGON_HEARDER_H
#define INTERFACE_POLYGON_HEARDER_H
#include <vector>
#include "IDrawShape.h"
using namespace std;

/**
 * @brief 场景节点类
 */
class IPolygon:public IDrawShape
{
public:

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
    virtual void SetMultPos(const vector<ScenePos>&)=0;
    virtual vector<ScenePos> GetMulPos()=0;

    /**
     * @brief 设置点的大小
     */
    virtual void SetColor(const SceneColor&)=0;
    virtual const SceneColor& GetColor()=0;
};

#endif
