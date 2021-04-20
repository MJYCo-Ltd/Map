#ifndef CATMOSPHERE_H
#define CATMOSPHERE_H
#include <osg/Node>
#include <Plot/Common/ISceneCameraGroup.h>
#include <Plot/SceneShape/IEllipsoidSensor.h>

class CAtmosphere
{
public:
    CAtmosphere(ISceneGraph* pSceneGraph,int nIndex);

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
    ISceneCameraGroup*   m_pCameraNode;
    IEllipsoidSensor*    m_pEllipsoid;
    ISceneGraph*         m_pSceneGraph;
    int                  m_nIndex;
};

#endif // CATMOSPHERE_H
