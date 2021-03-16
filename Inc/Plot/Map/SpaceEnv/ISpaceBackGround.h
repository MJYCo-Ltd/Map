#ifndef INTERFACE_SPACE_BACKGROUBD_HEARDER_H
#define INTERFACE_SPACE_BACKGROUBD_HEARDER_H

#include <SceneGraph/ISceneNode.h>
#include <Math/Matrix.h>
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
     * @brief 更新时间
     * @param 约简儒略日
     */
    void UpdateDate(double dMJD)JUDGE_DOUBLE_CALL_FUNCTION(dMJD,m_dMJD,DateChanged)

    /**
     * @brief 更新矩阵
     */
    virtual void UpdateMatrix(const Math::CMatrix& rRotate)JUDGE_EQUAL_CALL_FUNCTION(rRotate,m_matRotate,MatrixChanged)

    /**
     * @brief 是否开启大气模型
     * @param bShow
     */
    virtual void ShowAtmosphere(bool)=0;
protected:
    virtual ~ISpaceBackGround(){}
    virtual void DateChanged()=0;
    virtual void MatrixChanged()=0;
protected:
    double m_dMJD=0;
    Math::CMatrix m_matRotate;
};

#endif
