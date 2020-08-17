#ifndef SOLARENV_H
#define SOLARENV_H

#include <osg/Matrix>
#include "PlanetModel.h"
#include "SunModel.h"

class ISceneGraph;

class CSolarEnv:public osg::MatrixTransform
{
public:
    CSolarEnv(ISceneGraph* pSceneGraph);

    /**
     * @brief 更新矩阵
     * @param crMatrix
     */
    void UpdateMatrix(const osg::Matrix& crMatrix);
    const osg::Matrix& GetUpdateMatrix(){return(m_rRotMatrix);}

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
     * @brief 更新时间
     * @param dMJD
     */
    void UpdateTime(const double& dMJD);

    /**
     * @brief 更新
     * @param nv
     */
    void traverse(osg::NodeVisitor &nv);

    /**
     * @brief 设置行星名称显隐
     * @param bVisible
     */
    void SetPlanetNameShow(bool bVisible);
private:
    ISceneGraph*     m_pSceneGraph;                       /// 场景
    bool             m_bNeedUpdate;                       /// 是否需要更新
    osg::Matrix      m_rRotMatrix;                        /// 旋转矩阵
    std::map<int,osg::ref_ptr<CPlanetModel> > m_mapPlanet;/// 行星绘制类
    osg::ref_ptr<CSunModel>                   m_pSun;     /// 绘制的太阳
};

#endif // SOLARENV_H
