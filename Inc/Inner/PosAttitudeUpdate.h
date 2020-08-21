#ifndef HEADER_POS_ATTITUDE_UPDATE_H
#define HEADER_POS_ATTITUDE_UPDATE_H

#include <osg/PositionAttitudeTransform>
#include <Sofa/sofam.h>
#include <Math/VecMat.h>
#include <Math/YPRAngle.h>
#include <Math/Quaternion.h>
#include <Inner/OsgExtern/MyPositionAttitudeTransform.h>
#include "SceneType.h"

/// 位置和姿态更新
class PosAttitudeUpdate:public osg::Callback
{
public:
    PosAttitudeUpdate(CMyPositionAttitudeTransform* pTransForm):m_pTransform(pTransForm){}

    /**
     * @brief 更新位置
     */
    void SetPos(const osg::Vec3d& vPos)
    {
        m_vPos = vPos;
        m_bUpdatePos = true;
    }

    void SetMatrix(const osg::Matrix& rRotateMat)
    {
        m_matRotate = rRotateMat;
        m_bUpdateRotate = true;
    }

    /**
     * @brief 更新姿态
     */
    void SetAttitude(const SceneAttitude& rAttitude)
    {
        Math::YPRROTATE type=YPR;
        switch (rAttitude.rotaOrder)
        {
        case SR_RPY:
        case SR_XYZ:
            type = RPY;
            break;
        case SR_RYP:
        case SR_XZY:
            type = RYP;
            break;
        case SR_PRY:
        case SR_YXZ:
            type = PRY;
            break;
        case SR_PYR:
        case SR_YZX:
            type = PYR;
            break;
        case SR_YRP:
        case SR_ZXY:
            type = YRP;
            break;
        default:
            break;
        }
        Math::CMatrix RoateMatrix = Math::CYPRAngle::CreateMatrix(rAttitude.dRoll*DD2R,
                                      rAttitude.dPitch*DD2R,
                                      rAttitude.dYaw*DD2R,
                                      type);

        m_matRotateAttitude.set(RoateMatrix(0,0),RoateMatrix(0,1),RoateMatrix(0,2),0.,
                        RoateMatrix(1,0),RoateMatrix(1,1),RoateMatrix(1,2),0.,
                        RoateMatrix(2,0),RoateMatrix(2,1),RoateMatrix(2,2),0.,
                        0.,0.,0.,1.);

        m_bUpdateRotate = true;
    }

    void SetPivo(const osg::Vec3d& vPos)
    {
        m_vPivot = vPos;
        m_bUpdatePivot = true;
    }

    /// 更新循环
    bool run(osg::Object* object, osg::Object* data)
    {
        if(m_bUpdatePos)
        {
            m_pTransform->setPosition(m_vPos);
            m_bUpdatePos = false;
        }

        if(m_bUpdateRotate)
        {
            m_pTransform->SetRotateMatrix(m_matRotateAttitude*m_matRotate);
            m_bUpdateRotate = false;
        }

        if(m_bUpdateScale)
        {
            m_pTransform->setScale(osg::Vec3d(m_dScale,m_dScale,m_dScale));
            m_bUpdateScale = false;
        }

        if (m_bUpdatePivot)
        {
            m_pTransform->setPivotPoint(m_vPivot);
            m_bUpdatePivot = false;
        }
        return traverse(object, data);
    }

    /// 更新缩放
    void SetScal(double dScale)
    {
        m_dScale = dScale;
        m_bUpdateScale = true;
    }
private:
    CMyPositionAttitudeTransform* m_pTransform;
    osg::Vec3d  m_vPos;
    osg::Vec3d  m_vPivot;
    osg::Matrix m_matRotate;
    osg::Matrix m_matRotateAttitude;
    double      m_dScale=1.0;
    bool  m_bUpdatePos=false;
    bool  m_bUpdateRotate=false;
    bool  m_bUpdateScale=false;
    bool  m_bUpdatePivot=false;
};

#endif // QT_OSG_SCENE_NODE_H
