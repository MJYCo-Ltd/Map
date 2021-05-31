#ifndef INTERFACE_MAP_POLYGON_HEARDER_H
#define INTERFACE_MAP_POLYGON_HEARDER_H
#include <Plot/Map/IMapSceneNode.h>
#include <Plot/SceneShape/IPolygon.h>

/**
 * @brief 场景节点类
 */
class IMapPolygon:public IMapSceneNode
{
public:
    CONSTRUCTOR(IMapPolygon,IMapSceneNode)

    /**
     * @brief 获取绘制信息
     * @return
     */
    IPolygon* GetDrawPolygon(){return(m_pDrawPolygon);}
protected:
    virtual ~IMapPolygon(){}
protected:
    IPolygon* m_pDrawPolygon{};
};

#endif
