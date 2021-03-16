#ifndef CATMOSPHERE_H
#define CATMOSPHERE_H

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
protected:
    ISceneCameraGroup*   m_pCameraNode;
    IEllipsoidSensor*    m_pEllipsoid;
    ISceneGraph*         m_pSceneGraph;
    int                  m_nIndex;
};

#endif // CATMOSPHERE_H
