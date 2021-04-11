#ifndef H_HEADER_MAP_POINT_H
#define H_HEADER_MAP_POINT_H

#include <Plot/Map/IMapPoint.h>
#include <Inner/ImplMapSceneGeometry.hpp>

class CMapPoint:public ImplMapSceneGeometry<IMapPoint>
{
public:
    CONSTRUCTOR(CMapPoint,ImplMapSceneGeometry<IMapPoint>)

    /**
     * @brief 初始化节点
     */
    void InitNode() override;
};

#endif // H_HEADER_MAP_POINT_H
