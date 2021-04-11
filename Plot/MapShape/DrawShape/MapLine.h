#ifndef H_HEADER_CSCENELINE_H
#define H_HEADER_CSCENELINE_H

#include <osgEarth/FeatureNode>
#include <Plot/Map/IMapLine.h>
#include <Inner/ImplMapSceneGeometry.hpp>

class CMapLine:public ImplMapSceneGeometry<IMapLine>
{
public:
    CONSTRUCTOR(CMapLine,ImplMapSceneGeometry<IMapLine>)

    /**
     * @brief 初始化节点
     */
    void InitNode() override;
};

#endif // CSCENELINE_H
