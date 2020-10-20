#ifndef INTERFACE_SCENE_ATTITUDE_GROUP_HEARDER_H
#define INTERFACE_SCENE_ATTITUDE_GROUP_HEARDER_H
#include <Math/Matrix.h>
#include <SceneGraph/ISceneGroup.h>

/**
 * @brief 场景节点类
 */
class ISceneAttitudeGroup:public ISceneGroup
{
public:
    CONSTRUCTOR(ISceneAttitudeGroup,ISceneGroup)

    ISceneAttitudeGroup* AsSceneAttitudeGroup(){return(this);}

    /**
     * @brief 设置场景节点的位置
     */
    void SetPos(const ScenePos& rPos)JUDGE_EQUAL_CALL_FUNCTION(rPos,m_stPos,PosChanged)
    const ScenePos& GetPos() const{return(m_stPos);}

    /**
     * @brief 设置姿态
     */
    void SetAttitude(const SceneAttitude& rAttitude)JUDGE_EQUAL_CALL_FUNCTION(rAttitude,m_stAttitude,AttitudeChanged)
    const SceneAttitude& GetAttitude()const{return(m_stAttitude);}
    void SetAttitude(const Math::CMatrix& rMatrix)JUDGE_EQUAL_CALL_FUNCTION(rMatrix,m_matAttitude,AttitudeMatrixChanged)
    const Math::CMatrix& Attitude()const{return(m_matAttitude);}

    /**
     * @brief 设置旋转依赖的中心点
     * @attention 例如人的手是相对于肘关节进行
                  旋转，而不是手腕
     */
    void SetPivotPos(const ScenePos& rPivotPos)JUDGE_EQUAL_CALL_FUNCTION(rPivotPos,m_stPivot,PivotPosChanged)
    const ScenePos& PivotPos() const{return(m_stPivot);}
protected:
    virtual ~ISceneAttitudeGroup(){}

    virtual void PosChanged()=0;
    virtual void AttitudeChanged()=0;
    virtual void AttitudeMatrixChanged()=0;
    virtual void PivotPosChanged()=0;
protected:
    Math::CMatrix m_matAttitude;
    ScenePos      m_stPos;
    ScenePos      m_stPivot;
    SceneAttitude m_stAttitude;
};

#endif // INTERFACE_SCENE_ATTITUDE_GROUP_HEARDER_H
