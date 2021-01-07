#ifndef CSCENERADAR_H
#define CSCENERADAR_H

#include <Plot/SceneShape/IRadarSensor.h>
#include <Inner/ImplSceneSensor.hpp>
class CSceneRadar:public ImplSceneSensor<IRadarSensor>
{
public:
    CONSTRUCTOR(CSceneRadar,ImplSceneSensor<IRadarSensor>)

protected:
    /**
     * @brief 更新形状
     */
    void UpdateShape();

    /**
     * @brief 构造模型
     */
    void CreateShape();
protected:
    osg::ref_ptr<osg::DrawElementsUShort> m_pDrawSideFace;
};

#endif // CSCENERADAR_H
