#ifndef INTERFACE_SPACE_BACKGROUBD_HEARDER_H
#define INTERFACE_SPACE_BACKGROUBD_HEARDER_H

#include <vector>
#include <Math/VecMat.h>
#include <SceneGraph/ISceneNode.h>

/**
 * @brief 场景节点类
 */
class ISpaceBackGround:public ISceneNode
{
public:
    CONSTRUCTOR(ISpaceBackGround,ISceneNode)

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
     * @brief 不知道怎么用的情况下不要擅自调用
     */
    virtual void UpdateMatrix(const Math::CMatrix&)=0;
    virtual void UpdatePos(const std::vector<Math::CVector>&)=0;
protected:
    virtual ~ISpaceBackGround(){}
};

#endif
