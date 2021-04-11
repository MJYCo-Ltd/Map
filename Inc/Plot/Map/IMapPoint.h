#ifndef INTERFACE_MAP_POINT_HEARDER_H
#define INTERFACE_MAP_POINT_HEARDER_H

#include <Plot/Map/IMapSceneNode.h>
#include <Plot/SceneShape/IPoint.h>
/**
 * @brief 场景节点类
 */
class IMapPoint:public IMapSceneNode
{
public:
    CONSTRUCTOR(IMapPoint,IMapSceneNode)
    /**
     * @brief 获取真实的绘制体
     * @return
     */
    IPoint* GetDrawPoint(){return(m_pDrawPoint);}
protected:
    ~IMapPoint(){}
protected:
    IPoint* m_pDrawPoint=nullptr;
};

#endif
