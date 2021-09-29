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
protected:
//    osg::ref_ptr<osg::Vec2Array> m_pTexCoords;
    osg::ref_ptr<osg::Vec3Array> m_pNormals;
};

#endif // CSCENEBOX_H
