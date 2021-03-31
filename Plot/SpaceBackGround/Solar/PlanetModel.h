#ifndef PLANETMODEL_H
#define PLANETMODEL_H

#include <string>
#include <osg/Node>
#include <Plot/Common/ISceneAttitudeGroup.h>
#include <Plot/Common/ISceneLodGroup.h>
#include <Plot/Common/ISceneCameraGroup.h>
#include <Plot/SceneShape/IPoint.h>
#include <Plot/SceneShape/IEllipsoidSensor.h>
class ISceneGraph;

class CPlanetModel
{
public:
    CPlanetModel(ISceneGraph* pSceneGraph,int nIndex);
    ~CPlanetModel();

    /**
     * @brief 更新行星位置
     * @param rPos
     */
    void UpdatePostion(const Math::CVector& rPos);

    /**
     * @brief 获取显示节点
     * @return
     */
    osg::Node* GetNode();

protected:
    /**
     * @brief 构建行星
     */
    void MakePlanet();
private:
    ISceneAttitudeGroup* m_pAttitudeNode;
    ISceneLodGroup*      m_pLodNode;
    ISceneCameraGroup*   m_pCameraNode;
    IPoint*              m_pPointNode;
    IEllipsoidSensor*    m_pEllipsoid;

    int          m_nRenderBin;
    ISceneGraph* m_pSceneGraph;
    float       m_fEquatorRadius;                   /// 赤道半径
    float       m_fPolarRadius;                     /// 极半径
    osg::Vec4    m_rColor;                          /// 颜色
    std::string  m_cnPlanetName;                    /// 行星显示名称
    std::string  m_sPlanetImage;                    /// 行星图片名称
};

#endif // PLANETMODEL_H
