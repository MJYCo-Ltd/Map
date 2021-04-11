#ifndef CHEADER_SENEPOLYGON_H
#define CHEADER_SENEPOLYGON_H

#include <Plot/Map/IMapPolygon.h>
#include <Inner/ImplMapSceneGeometry.hpp>

class CMapPolygon:public ImplMapSceneGeometry<IMapPolygon>
{
public:
    CONSTRUCTOR(CMapPolygon,ImplMapSceneGeometry<IMapPolygon>)

    /**
     * @brief 初始化节点
     */
    void InitNode() override;
};

#endif // CSENEPOLYGON_H
