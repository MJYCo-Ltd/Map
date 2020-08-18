#ifndef QT_OSG_SCENE_NODE_H
#define QT_OSG_SCENE_NODE_H

#include <set>
#include <osg/PositionAttitudeTransform>
#include <algorithm>
#include <osg/OperationThread>

#include <Sofa/sofam.h>
#include <Math/VecMat.h>
#include <Math/YPRAngle.h>
#include <Math/Quaternion.h>

#include <Inner/IRender.h>
#include <Inner/OsgExtern/OsgExtern.h>
#include <Inner/OsgExtern/MyPositionAttitudeTransform.h>

#include "IOsgSceneNode.h"
#include "OsgExtern/OsgExtern_Global.h"
#include "SceneGraph/ISceneGraph.h"
using namespace std;
class QtSceneGraph;

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

/**
 *  实现ISceneNode所有的接口
 */
template <typename T>
class QtOsgSceneNode:public T,public IOsgSceneNode
{
public:
    explicit QtOsgSceneNode(ISceneGraph* pSceneGraph)
    {
        m_stScenePos.bIsGeo = false;
        T::m_pSceneGraph = pSceneGraph;
    }

    virtual ~QtOsgSceneNode()
    {
        T::m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new RemoveFromeScene(m_pOsgNode));
        m_pOsgNode = nullptr;
    }

    /**
     * @brief 设置场景节点的位置
     */
    void SetPos(const ScenePos&rPos)
    {
        if(m_stScenePos != rPos)
        {
            m_stScenePos = rPos;
            PosChanged();
        }
    }

    /**
     * @brief 获取当前节点位置
     * @return 当前节点位置
     */
    const ScenePos& GetPos() const
    {
        return(m_stScenePos);
    }

    /**
     * @brief 设置姿态
     * @param rAttitude
     */
    void SetAttitude(const SceneAttitude& rAttitude)
    {
        if(m_stAttitude != rAttitude)
        {
            m_stAttitude = rAttitude;
            AttitudeChanged();
        }
    }

    const SceneAttitude& GetAttitude() const
    {
        return(m_stAttitude);
    }

    /**
     * @brief 设置缩放系数
     */
    void SetScal(double dScal)
    {
        if(dScal > 0 && dScal != m_dScal)
        {
            m_dScal = dScal;
        }
    }
    double Scal()const
    {
        return(m_dScal);
    }

    /**
     * @brief 设置旋转依赖的中心点
     * @attention 例如人的手是相对于肘关节进行
                  旋转，而不是手腕
     */
    void SetPivotPos(const ScenePos& rPivoPos)
    {
        if(m_stPivoPos != rPivoPos)
        {
            m_stPivoPos = rPivoPos;
        }
    }

    const ScenePos& PivotPos() const
    {
        return(m_stPivoPos);
    }

    /**
     * @brief 添加场景节点
     */
    bool AddSceneNode(ISceneNode*rSceneNode)
    {
        if(nullptr == rSceneNode)
        {
            return(false);
        }

        /// 如果两者不是同一个渲染器,则返回
        if(rSceneNode->GetBindSceneGraph() != T::m_pSceneGraph)
        {
            return(false);
        }

        IOsgSceneNode* pOsgSceneNode = dynamic_cast<IOsgSceneNode*>(rSceneNode);

        auto findItor = m_setChildNode.find(pOsgSceneNode);
        if(findItor == m_setChildNode.end())
        {
            auto update = new CModifyNode(m_pOsgNode,pOsgSceneNode->GetOsgNode(),true);
            T::m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(update);

            m_setChildNode.insert(pOsgSceneNode);
            return(true);
        }

        return(false);
    }

    /**
     * @brief 删除场景节点
     * @return 是否删除节点
     */
    bool RemoveSceneNode(ISceneNode* rSceneNode)
    {
        IOsgSceneNode* pOsgSceneNode = dynamic_cast<IOsgSceneNode*>(rSceneNode);
        auto findItor = m_setChildNode.find(pOsgSceneNode);
        if(findItor != m_setChildNode.end())
        {
            auto update = new CModifyNode(m_pOsgNode,pOsgSceneNode->GetOsgNode(),false);
            T::m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(update);

            m_setChildNode.erase(findItor);
            return(true);
        }

        return(false);
    }

    /**
     * @brief 初始化场景节点
     */
    void InitSceneNode()
    {
        m_pOsgNode = new CMyPositionAttitudeTransform;
        m_pUpdataCall = new PosAttitudeUpdate(m_pOsgNode);
        m_pOsgNode->addUpdateCallback(m_pUpdataCall);
    }

    /**
     * @brief 获取osg的节点
     * @return
     */
    osg::Group* GetOsgNode()
    {
        return(m_pOsgNode.get());
    }

    /**
     * @brief 设置节点是否可见
     * @param bVisible
     */
    void SetVisible(bool bVisible)
    {
        if(m_bVisible != bVisible)
        {
            m_bVisible = bVisible;
            if(m_bVisible)
            {
                /// 此处可能会导致遍历器设置mask的问题
                m_pOsgNode->setNodeMask(-1);
            }
            else
            {
                m_pOsgNode->setNodeMask(0);
            }

            VisibleChanged();
        }
    }

    bool IsVisible() const{return(m_bVisible);}

    /**
     * @brief 是否可以删除
     * @return
     */
    virtual bool CanDelete() const
    {
        return(m_pOsgNode->referenceCount() < 2);
    }

protected:

    /**
     * @brief 显隐状态更改
     */
    virtual void VisibleChanged(){}
    /**
     * @brief 位置更改
     */
    virtual void PosChanged()
    {
        if(!m_stScenePos.bIsGeo && nullptr != m_pUpdataCall)
        {
            m_pUpdataCall->SetPos(osg::Vec3d(m_stScenePos.fX,m_stScenePos.fY,m_stScenePos.fZ));
        }
    }

    /**
     * @brief 姿态更改
     */
    virtual void AttitudeChanged()
    {
        if(nullptr != m_pUpdataCall)
        {
            m_pUpdataCall->SetAttitude(m_stAttitude);
        }
    }

protected:
    set<IOsgSceneNode*>               m_setChildNode;/// 子节点
    osg::ref_ptr<CMyPositionAttitudeTransform>  m_pOsgNode;    /// 本节点
    ScenePos                          m_stScenePos;  /// 场景位置
    ScenePos                          m_stPivoPos;   /// 相对中心位置
    SceneAttitude                     m_stAttitude;  /// 姿态信息
    PosAttitudeUpdate*                m_pUpdataCall=nullptr; /// 更新回调
    double                            m_dScal=1.0;
    bool                              m_bVisible=true;/// 是否可见
    bool                              m_bCanDelete=true;///是否可以删除
};

#endif // QT_OSG_SCENE_NODE_H
