#ifndef IMPL_SCENE_ATTITUDE_GROUP_H
#define IMPL_SCENE_ATTITUDE_GROUP_H
#include <VersionMathCommon.h>
#include <Inner/OsgExtern/MyPositionAttitudeTransform.h>
#include <Math/YPRAngle.h>
#include <Inner/Common/ImplSceneGroup.hpp>

/**
 *  实现ISceneAttitudeGroup的接口
 */
template <typename T>
class ImplSceneAttitudeGroup:public ImplSceneGroup<T>
{
public:
    CONSTRUCTOR(ImplSceneAttitudeGroup,ImplSceneGroup<T>)

protected:
    void UpdateNode()
    {
        if(m_bPosChanged)
        {
            m_pOsgAttitudeNode->setPosition(osg::Vec3d(T::m_stPos.fX,T::m_stPos.fY,T::m_stPos.fZ));
            m_bPosChanged=false;
        }

        if(m_bAttitudeChanged)
        {
            Math::YPRROTATE type=Math::YPR;
            switch (T::m_stAttitude.rotaOrder)
            {
            case SR_RPY:
            case SR_XYZ:
                type = Math::RPY;
                break;
            case SR_RYP:
            case SR_XZY:
                type = Math::RYP;
                break;
            case SR_PRY:
            case SR_YXZ:
                type = Math::PRY;
                break;
            case SR_PYR:
            case SR_YZX:
                type = Math::PYR;
                break;
            case SR_YRP:
            case SR_ZXY:
                type = Math::YRP;
                break;
            default:
                break;
            }
            T::m_matAttitude = Math::CYPRAngle::CreateMatrix(T::m_stAttitude.dRoll*DD2R,
                                                             T::m_stAttitude.dPitch*DD2R,
                                                             T::m_stAttitude.dYaw*DD2R,
                                                             type);

            m_pOsgAttitudeNode->SetRotateMatrix(T::m_matAttitude);
            m_bAttitudeChanged=false;
        }

        if(m_bAttitudeMatrixChanged)
        {
            m_pOsgAttitudeNode->SetRotateMatrix(T::m_matAttitude);
            m_bAttitudeMatrixChanged = false;
        }

        if(m_bPivotChanged)
        {
            m_pOsgAttitudeNode->setPivotPoint(osg::Vec3(T::m_stPivot.fX,T::m_stPivot.fY,T::m_stPivot.fZ));
            m_bPivotChanged=false;
        }

        ImplSceneGroup<T>::UpdateNode();
    }

    /**
     * @brief 重写节点初始化
     */
    void InitNode()
    {
        ImplSceneNode<T>::InitNode();
        m_pOsgAttitudeNode = new CMyPositionAttitudeTransform;
        ImplSceneGroup<T>::SetGroupNode(m_pOsgAttitudeNode.get());
    }

    /// 状态更改
    void PosChanged()SET_TRUE_NODE_UPDATE(m_bPosChanged)
    void AttitudeChanged()SET_TRUE_NODE_UPDATE(m_bAttitudeChanged)
    void AttitudeMatrixChanged()SET_TRUE_NODE_UPDATE(m_bAttitudeMatrixChanged)
    void PivotPosChanged()SET_TRUE_NODE_UPDATE(m_bPivotChanged)
protected:
    bool m_bPosChanged=false;
    bool m_bAttitudeChanged=false;
    bool m_bAttitudeMatrixChanged=false;
    bool m_bPivotChanged=false;
    osg::observer_ptr<CMyPositionAttitudeTransform>   m_pOsgAttitudeNode; /// 本节点
};

#endif // IMPL_SCENE_ATTITUDE_GROUP_H