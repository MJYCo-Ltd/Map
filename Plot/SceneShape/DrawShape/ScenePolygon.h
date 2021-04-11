#ifndef CSCENEPOLYGON_H
#define CSCENEPOLYGON_H

#include <list>
#include <osgUtil/Tessellator>
#include <Plot/SceneShape/IPolygon.h>
#include <Inner/ImplSceneGeometry.hpp>

class CScenePolygon:public ImplSceneGeometry<IPolygon>
{
public:
    CONSTRUCTOR(CScenePolygon,ImplSceneGeometry<IPolygon>)

protected:
    void CreateShape();
    void UpdateShape();
protected:
    osg::ref_ptr<osgUtil::Tessellator> m_pTess;
};

#endif // CSCENEPOLYGON_H
