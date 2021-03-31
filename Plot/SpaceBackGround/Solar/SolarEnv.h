#ifndef SOLARENV_H
#define SOLARENV_H

#include <osg/Matrix>
#include "PlanetModel.h"
#include "SunModel.h"

class ISceneGraph;
class CAtmosphere;

class CSolarEnv:public osg::Group
{
public:
    CSolarEnv(ISceneGraph* pSceneGraph);
    ~CSolarEnv();

    /**
     * @brief 创造整个太阳系
     */
    void CreateSolar();

    /**
     * @brief 是否需要更新
     * @return
     */
    bool& IsNeedUpdate(){return(m_bNeedUpdate);}

    /**
     * @brief 更新行星位置
     * @param 行星位置
     */
    void UpdatePos(const std::vector<Math::CVector> &vSolarPos);

    /**
     * @brief 设置行星名称显隐
     * @param bVisible
     */
    void SetPlanetNameShow(bool bVisible);

    /**
     * @brief 是否显示大气
     */
    void ShowAtmosphere(bool);
private:
    ISceneGraph*     m_pSceneGraph;          /// 场景
    bool             m_bNeedUpdate;          /// 是否需要更新
    std::map<int,CPlanetModel*> m_mapPlanet; /// 行星绘制类
    osg::ref_ptr<CSunModel>     m_pSun;      /// 绘制的太阳
    CAtmosphere*                m_pAtmosphere;
};

#endif // SOLARENV_H
