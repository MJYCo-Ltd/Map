#ifndef SCENEDONUTS_H
#define SCENEDONUTS_H

#include <Inner/ImplSceneSensor.hpp>
#include <Plot/SceneShape/ISceneDonut.h>
#include <Inner/OsgExtern/MathCommon.h>
#include <Inner/OsgExtern/Grid2TriangleStrip.h>


class CSceneDonuts:public ImplSceneSensor<ISceneDonut>
{
public:
    CONSTRUCTOR(CSceneDonuts,ImplSceneSensor<ISceneDonut>)
protected:
    void CreateShape();
    void UpdateShape();
};

#endif // SCENEDONUTS_H
