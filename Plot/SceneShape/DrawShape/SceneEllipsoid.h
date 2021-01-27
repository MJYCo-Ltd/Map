#ifndef CSCENEELLIPSOID_H
#define CSCENEELLIPSOID_H

#include <Plot/SceneShape/IEllipsoidSensor.h>
#include <Inner/ImplSceneSensor.hpp>
class CSceneEllipsoid:public ImplSceneSensor<IEllipsoidSensor>
{
public:
    CONSTRUCTOR(CSceneEllipsoid,ImplSceneSensor<IEllipsoidSensor>)

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
    osg::ref_ptr<osg::EllipsoidModel> m_pEllipsoidModel;
    osg::ref_ptr<osg::Vec2Array> m_pTexCoords;
    osg::ref_ptr<osg::Vec3Array> m_pNormals;
    osg::ref_ptr<osgEarth::VirtualProgram> m_pVirutlProgram;
};

#endif // CSCENEELLIPSOID_H
