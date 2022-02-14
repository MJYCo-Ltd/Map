#ifndef CBEAMSENSOR_H
#define CBEAMSENSOR_H
#include <Plot/SceneShape/IBeamSensor.h>
#include <Inner/ImplSceneSensor.hpp>

/**
 * @brief The CBeamSensor class
 */
class CBeamSensor:public ImplSceneSensor<IBeamSensor>
{
public:
    CONSTRUCTOR(CBeamSensor,ImplSceneSensor<IBeamSensor>)

protected:
    void CreateShape();
    void UpdateShape();
};

#endif // CBEAMSENSOR_H
