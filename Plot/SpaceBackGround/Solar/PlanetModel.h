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
    const osg::Vec3& GetUpdatePostion(){return(m_rECIPostion);}


    /**
     * @brief 设置名字是否可见
     * @param bVisble
     */
    void SetNameVisible(bool bVisble);

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
    osg::Vec3    m_rECIPostion;                     /// 位置
    osg::Vec4    m_rColor;                          /// 颜色
    std::wstring m_wsPlanetName;                    /// 行星显示名称
    std::string  m_sPlanetImage;                    /// 行星图片名称
};

#endif // PLANETMODEL_H
