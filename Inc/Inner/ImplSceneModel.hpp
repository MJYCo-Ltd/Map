#ifndef IMPL_SCENE_MODEL_H
#define IMPL_SCENE_MODEL_H
#include <Sofa/sofam.h>
#include <Math/Matrix.h>
#include <Math/YPRAngle.h>
#include <Inner/OsgExtern/MyMatrixTransform.h>
#include <Inner/ImplSceneNode.hpp>

/**
 *  实现ISceneModel的所有的接口
 */
template <typename T>
class ImplSceneModel:public ImplSceneNode<T>
{
public:
    CONSTRUCTOR(ImplSceneModel,ImplSceneNode<T>)

    /**
     * @brief设置osg节点
     */
    void SetModel(osg::Node* pNode)
    {
        m_pModel = pNode;
        m_pInitAttitudeMatrix->addChild(m_pModel);
    }
protected:

    void UpdateNode()
    {
        Math::YPRROTATE type=Math::YPR;
        switch (T::m_stInitAttitude.rotaOrder)
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

        m_pInitAttitudeMatrix->SetMatrix(Math::CYPRAngle::CreateMatrix(T::m_stInitAttitude.dRoll*DD2R,
                                                                       T::m_stInitAttitude.dPitch*DD2R,
                                                                       T::m_stInitAttitude.dYaw*DD2R,
                                                                       type));

        ImplSceneNode<T>::UpdateNode();
    }
    /**
     * @brief 初始化节点
     */
    void InitNode()
    {
        ImplSceneNode<T>::InitNode();
        m_pInitAttitudeMatrix = new CMyMatrixTransform;
        SetOsgNode(m_pInitAttitudeMatrix.get());
    }

    /**
     * @brief 显隐状态更改
     */
    void InitAttitudeChanged()
    {
        NodeChanged();
    }

protected:
    osg::ref_ptr<osg::Node>               m_pModel;
    osg::observer_ptr<CMyMatrixTransform> m_pInitAttitudeMatrix;
};

#endif // IMPL_SCENE_MODEL_H
