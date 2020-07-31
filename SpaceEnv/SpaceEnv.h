#ifndef SPACEENV_H
#define SPACEENV_H

#include <SpaceEnv/ISpaceEnv.h>
#include <Inner/QtOsgSceneNode.h>
#include "SpaceEnv_Global.h"

class CSolarEnv;
class CStarEnv;

class CSpaceEnv:public QtOsgSceneNode<ISpaceEnv>
{
    friend class CMatixUpdateCallback;

public:
    CSpaceEnv(ISceneGraph* pSceneGraph);
    ~CSpaceEnv();
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
     * @brief 初始化节点
     */
    void InitSceneNode();
protected:
    void  UpdateMatrix();
protected:
    osg::ref_ptr<CSolarEnv> m_pSolarEnv;  /// 行星背景
    osg::ref_ptr<CStarEnv>  m_pStarEnv;   /// 星空背景
    CMatixUpdateCallback* m_pUpdateCallBack;
    osg::ref_ptr<osg::MatrixTransform> m_pRotateTransform;
    double     m_dMJD=0.0;       /// 约简儒略日
};

extern "C"
{
    SPACEENVSHARED_EXPORT ISpaceEnv* CreateSpaceEnv(ISceneGraph* pSceneGraph);
}
#endif // SPACEENV_H
