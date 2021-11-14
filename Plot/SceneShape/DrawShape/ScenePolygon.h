#ifndef CSCENEPOLYGON_H
#define CSCENEPOLYGON_H

#include <Plot/SceneShape/IPolygon.h>
#include <Inner/ImplSceneGeometry.hpp>

class CScenePolygon:public ImplSceneGeometry<IPolygon>
{
public:
    CONSTRUCTOR(CScenePolygon,ImplSceneGeometry<IPolygon>)

protected:
    void UpdateVetrix();
    void CreateShape();
    void UpdateShape();
    void DrawChanged()SET_TRUE_SHAPE_UPDATE(m_bNoUse)
protected:
    osg::ref_ptr<osg::DrawArrays> m_pDrawLine;
    bool m_bNoUse{};
};

#endif // CSCENEPOLYGON_H
