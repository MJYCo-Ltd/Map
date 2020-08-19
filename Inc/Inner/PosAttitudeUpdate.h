#ifndef HEADER_POS_ATTITUDE_UPDATE_H
#define HEADER_POS_ATTITUDE_UPDATE_H

#include <osg/PositionAttitudeTransform>
#include <Sofa/sofam.h>
#include <Math/VecMat.h>
#include <Math/YPRAngle.h>
#include <Math/Quaternion.h>
#include "SceneType.h"

/// 位置和姿态更新
class PosAttitudeUpdate:public osg::Callback
{
public:
    PosAttitudeUpdate(osg::PositionAttitudeTransform* pTransForm):m_pTransform(pTransForm){}

    /**
     * @brief 更新位置
     */
    void SetPos(const osg::Vec3d& vPos)
    {
        m_vPos = vPos;
        m_bUpdatePos = true;
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
        Math::CQuaternion tmpQuat(Math::CYPRAngle::CreateMatrix(rAttitude.dRoll*DD2R,
                                      rAttitude.dPitch*DD2R,
                                      rAttitude.dYaw*DD2R,
                                      Math::YPR));

        m_qAttiutude.x() = tmpQuat.GetX();
        m_qAttiutude.y() = tmpQuat.GetY();
        m_qAttiutude.z() = tmpQuat.GetZ();
        m_qAttiutude.w() = tmpQuat.GetS();

        m_bUpdateAttitude = true;
    }

    /// 更新循环
    bool run(osg::Object* object, osg::Object* data)
    {
        if(m_bUpdatePos)
        {
            m_pTransform->setPosition(m_vPos);
            m_bUpdatePos = false;
        }

        if(m_bUpdateAttitude)
        {
            m_pTransform->setAttitude(m_qAttiutude);
            m_bUpdateAttitude = false;
        }

        if(m_bUpdateScale)
        {
            m_pTransform->setScale(osg::Vec3d(m_dScale,m_dScale,m_dScale));
            m_bUpdateScale = false;
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
    osg::PositionAttitudeTransform* m_pTransform;
    osg::Vec3d  m_vPos;
    osg::Quat   m_qAttiutude;
    double      m_dScale=1.0;
    bool  m_bUpdatePos=false;
    bool  m_bUpdateAttitude=false;
    bool  m_bUpdateScale=false;
};

#endif // QT_OSG_SCENE_NODE_H
