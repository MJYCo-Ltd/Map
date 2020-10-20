#ifndef CSCENESCONE_H
#define CSCENESCONE_H
#include <Plot/SceneShape/ISConeSensor.h>
#include <Inner/ImplSceneSensor.hpp>

class CSceneSCone:public ImplSceneSensor<ISConeSensor>
{
public:
    CONSTRUCTOR(CSceneSCone,ImplSceneSensor<ISConeSensor>)

protected:
    void CreateShape();
    void UpdateShape();
    void UpdateColor();
};

#endif // CSCENESCONE_H
