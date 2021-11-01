#ifndef CSCENEBOX_H
#define CSCENEBOX_H

#include <Plot/SceneShape/IBoxSensor.h>
#include <Inner/ImplSceneSensor.hpp>
/**
 * @brief 绘制盒子
 */
class CSceneBox:public ImplSceneSensor<IBoxSensor>
{
public:
    CONSTRUCTOR(CSceneBox,ImplSceneSensor<IBoxSensor>);
protected:
    void UpdateShape();

    void CreateShape();
};

#endif // CSCENEBOX_H
