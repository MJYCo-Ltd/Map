#ifndef IMPL_SCENE_ATTITUDE_GROUP_H
#define IMPL_SCENE_ATTITUDE_GROUP_H

#include "OsgExtern/MyPositionAttitudeTransform.h"
#include "OsgExtern/PosAttitudeUpdate.hpp"
#include "ImplSceneGroup.hpp"
using namespace std;

/**
 *  实现ISceneNode所有的接口
 */
template <typename T>
class ImplSceneAttitudeGroup:public ImplSceneGroup<T>
{
public:
    CONSTRUCTOR(ImplSceneAttitudeGroup,ImplSceneGroup<T>)

protected:

    /**
     * @brief 重写节点初始化
     */
    void InitNode()
    {
        m_pOsgAttitudeNode = new CMyPositionAttitudeTransform;
        SetOsgNode(m_pOsgAttitudeNode);
        m_pUpdataCall = new PosAttitudeUpdate(m_pOsgAttitudeNode);
        m_pOsgAttitudeNode->setUpdateCallback(m_pUpdataCall);

        ImplSceneGroup<T>::InitNode();
    }

    /// 状态更改
    void PosChanged(){m_pUpdataCall->SetPos(osg::Vec3d(T::m_stPos.fX, T::m_stPos.fY, T::m_stPos.fZ));}
    void AttitudeChanged(){m_pUpdataCall->SetAttitude(T::m_stAttitude);}
    void ScalChanged(){m_pUpdataCall->SetScal(T::m_dScalBit);}
    void PivotPosChanged(){m_pUpdataCall->SetPivo(osg::Vec3d(T::m_stPivot.fX, T::m_stPivot.fY, T::m_stPivot.fZ));}
protected:
    CMyPositionAttitudeTransform*   m_pOsgAttitudeNode;      /// 本节点
    PosAttitudeUpdate*              m_pUpdataCall = nullptr; /// 更新回调
};

#endif // IMPL_SCENE_ATTITUDE_GROUP_H
