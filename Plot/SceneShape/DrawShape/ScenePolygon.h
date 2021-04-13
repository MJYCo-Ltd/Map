#ifndef CSCENEPOLYGON_H
#define CSCENEPOLYGON_H

#include <Plot/SceneShape/IPolygon.h>
#include <Inner/ImplSceneGeometry.hpp>

class CScenePolygon:public ImplSceneGeometry<IPolygon>
{
public:
    CONSTRUCTOR(CScenePolygon,ImplSceneGeometry<IPolygon>)

protected:
    void CreateShape();
    void UpdateShape();
};

#endif // CSCENEPOLYGON_H
