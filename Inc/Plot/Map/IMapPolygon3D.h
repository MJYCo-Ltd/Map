#ifndef INTERFACE_MAP_POLYGON_3D_HEARDER_H
#define INTERFACE_MAP_POLYGON_3D_HEARDER_H
#include <Plot/Map/IMapSceneNode.h>
#include <Plot/SceneShape/IPolygon3D.h>

/**
 * @brief 场景节点类
 */
class IMapPolygon3D:public IMapSceneNode
{
public:
    CONSTRUCTOR(IMapPolygon3D,IMapSceneNode)

    /**
     * @brief 获取绘制信息
     * @return
     */
    IPolygon3D* GetDrawPolygon(){return(m_pDrawPolygon);}
protected:
    virtual ~IMapPolygon3D(){}
protected:
    IPolygon3D* m_pDrawPolygon{};
};

#endif//INTERFACE_MAP_POLYGON_3D_HEARDER_H
