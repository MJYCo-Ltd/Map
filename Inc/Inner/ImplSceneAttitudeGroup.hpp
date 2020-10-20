#ifndef IMPL_SCENE_ATTITUDE_GROUP_H
#define IMPL_SCENE_ATTITUDE_GROUP_H
#include <Sofa/sofam.h>
#include <Inner/OsgExtern/MyPositionAttitudeTransform.h>
#include <Math/YPRAngle.h>
#include <Inner/ImplSceneGroup.hpp>

/**
 *  实现ISceneNode所有的接口
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
            m_pOsgAttitudeNode->setPosition(osg::Vec3(T::m_stPos.fX,T::m_stPos.fY,T::m_stPos.fZ));
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
        m_pOsgAttitudeNode = new CMyPositionAttitudeTransform;
        SetOsgNode(m_pOsgAttitudeNode.get());
        ImplSceneGroup<T>::InitNode();
    }

    /// 状态更改
    void PosChanged(){NodeChanged();m_bPosChanged=true;}
    void AttitudeChanged(){NodeChanged();m_bAttitudeChanged=true;}
    void AttitudeMatrixChanged(){NodeChanged();m_bAttitudeMatrixChanged=true;}
    void PivotPosChanged(){NodeChanged();m_bPivotChanged=true;}
protected:
    bool m_bPosChanged=false;
    bool m_bAttitudeChanged=false;
    bool m_bAttitudeMatrixChanged=false;
    bool m_bPivotChanged=false;
    osg::observer_ptr<CMyPositionAttitudeTransform>   m_pOsgAttitudeNode; /// 本节点
};

#endif // IMPL_SCENE_ATTITUDE_GROUP_H
