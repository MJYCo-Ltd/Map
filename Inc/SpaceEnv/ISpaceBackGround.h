#ifndef INTERFACE_SPACE_BACKGROUBD_HEARDER_H
#define INTERFACE_SPACE_BACKGROUBD_HEARDER_H

#include "../SceneGraph/ISceneNode.h"



/**
 * @brief 场景节点类
 */
class ISpaceBackGround:public ISceneNode
{
public:
    /**
     * @brief 设置显示最大星等 默认为 6 最大星等为 20
     * @param nMax
     */
    virtual void SetMaxVisibleMagnitude(int)=0;

    /**
     * @brief 设置银河是否可见
     * @param bShow
     */
    virtual void SetMilkywayVisible(bool)=0;

    /**
     * @brief 设置星星名称是否可见
     * @param bShow
     */
    virtual void SetStarNamesVisible(bool)=0;

    /**
     * @brief 设置边界是否可见
     * @param bShow
     */
    virtual void SetBoundariesVisible(bool)=0;

    /**
     * @brief 设置星座连线是否可见
     * @param bShow
     */
    virtual void SetConstellationLinesVisible(bool)=0;

    /**
     * @brief 设置星座名称是否可见
     * @param bShow
     */
    virtual void SetConstellationNamesVisible(bool)=0;

    /**
     * @brief 设置行星名称是否可见
     * @param bShow
     */
    virtual void SetPlanetsNamesVisible(bool)=0;

    /**
     * @brief 更新时间
     * @param 约简儒略日
     */
    virtual void UpdateDate(double)=0;
};

#endif
