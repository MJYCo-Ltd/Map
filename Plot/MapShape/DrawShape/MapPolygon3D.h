#ifndef CMAPPOLYGON3D_H
#define CMAPPOLYGON3D_H

#include <Plot/Map/IMapPolygon3D.h>
#include <Inner/ImplMapSceneGeometry.hpp>

class CMapPolygon3D:public ImplMapSceneGeometry<IMapPolygon3D>
{
public:
    CONSTRUCTOR(CMapPolygon3D,ImplMapSceneGeometry<IMapPolygon3D>)

    /**
     * @brief 初始化节点
     */
    void InitNode() override;
};

#endif // CMAPPOLYGON3D_H
