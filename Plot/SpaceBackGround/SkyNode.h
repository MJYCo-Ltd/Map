#ifndef CSKYNODE_H
#define CSKYNODE_H
#include <osg/Group>
#include "Solar/SolarEnv.h"
#include "Star/StarEnv.h"

class CSkyNode:public osg::Group
{
public:
    CSkyNode(ISceneGraph* pSceneGraph);

    /**
     * @brief 设置显示最大星等 默认为 6 最大星等为 20
     * @param nMax
     */
    void SetMaxVisibleMagnitude(int nMax);

    /**
     * @brief 设置银河是否可见
     * @param bShow
     */
    void SetMilkywayVisible(bool bShow);

    /**
     * @brief 设置星星名称是否可见
     * @param bShow
     */
    void SetStarNamesVisible(bool bShow);

    /**
     * @brief 设置边界是否可见
     * @param bShow
     */
    void SetBoundariesVisible(bool bShow);

    /**
     * @brief 设置星座连线是否可见
     * @param bShow
     */
    void SetConstellationLinesVisible(bool bShow);

    /**
     * @brief 设置星座名称是否可见
     * @param bShow
     */
    void SetConstellationNamesVisible(bool bShow);

    /**
     * @brief 设置行星名称是否可见
     * @param bShow
     */
    void SetPlanetsNamesVisible(bool bShow);

    /**
     * @brief UpdateDate
     */
    void UpdateDate(double dMJD);

    /**
     * @brief 更新矩阵
     */
    void UpdateMatrix(const Math::CMatrix& rRotate);

    void ShowAtmosphere(bool);

public:
    ///////////////// 重写父类方法 ///////////////////
    void traverse(osg::NodeVisitor&);
    osg::BoundingSphere computeBound() const;
    void releaseGLObjects(osg::State* state) const;
    void resizeGLObjectBuffers(unsigned maxSize);
    ///////////////// 重写父类方法 end///////////////////
private:
    osg::ref_ptr<osg::Group> m_pSpaceBackGroundRoot;
    osg::ref_ptr<CSolarEnv>  m_pSolarEnv;  /// 行星背景
    osg::ref_ptr<CStarEnv>   m_pStarEnv;   /// 星空背景
    double                   m_dMJD=0.0;       /// 约简儒略日
};

#endif // CSKYNODE_H
