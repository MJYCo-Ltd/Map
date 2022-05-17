#ifndef SCENEDONUTS_H
#define SCENEDONUTS_H

#include <Inner/ImplSceneSensor.hpp>
#include <Plot/SceneShape/ISceneDonut.h>

class CSceneDonuts:public ImplSceneSensor<ISceneDonut>
{
public:
    CONSTRUCTOR(CSceneDonuts,ImplSceneSensor<ISceneDonut>)
protected:
    void CreateShape();
    void UpdateShape();
private:
    bool    m_bBuild;/// 是否构建完成
    osg::Vec4 m_vColor={1.0,0,0,1.0};
};

#endif // SCENEDONUTS_H
