#ifndef CATMOSPHERE_H
#define CATMOSPHERE_H
#include <osg/Node>
#include <Plot/Common/ISceneCameraGroup.h>
#include <Plot/SceneShape/IEllipsoidSensor.h>

class CAtmosphere
{
public:
    CAtmosphere(ISceneGraph* pSceneGraph);

    /**
     * @brief 构建太空盒子
     */
    void MakeAtmosphere();

    /**
     * @brief 获取节点
     * @return
     */
    osg::Node* GetNode();

    /**
     * @brief 设置是否可见
     * @param bVisilbe
     */
    void SetVisible(bool bVisilbe);
protected:
    IEllipsoidSensor*    m_pEllipsoid{nullptr};
    ISceneGraph*         m_pSceneGraph{nullptr};
};

#endif // CATMOSPHERE_H
