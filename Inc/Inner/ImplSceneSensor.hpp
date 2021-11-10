#ifndef IMPL_SCENE_SENSOR_H
#define IMPL_SCENE_SENSOR_H
#include <osg/PolygonMode>
#include <osg/CullFace>
#include <osg/MatrixTransform>
#include <Inner/ImplSceneShape.hpp>

/**
 *  实现ISceneSensor所有的接口
 */
template <typename T>
class ImplSceneSensor:public ImplSceneShape<T>
{
public:
    CONSTRUCTOR(ImplSceneSensor,ImplSceneShape<T>)

protected:

    /**
    * @brief 显示类型修改
    */
    void ShowTypeChanged()SET_TRUE_NODE_UPDATE(m_bShowTypeChanged)
    void DistanceChanged()SET_TRUE_NODE_UPDATE(m_bDistanceChanged)
    void EffectsChanged()SET_TRUE_NODE_UPDATE(m_bEffectChanged)
    void ShowBackChanged()SET_TRUE_NODE_UPDATE(m_bShowBackChanged)
    void CountChanged(){/*T::m_pSceneGraph->ResouceLoader()->GetOrCreateStateSet("GLSL/Pulse.glsl")->
                getOrCreateUniform("fPulseStep",osg::Uniform::FLOAT)->set(1.f/T::m_unCount);*/}
    void DirectionChanged(){/*T::m_pSceneGraph->ResouceLoader()->GetOrCreateStateSet("GLSL/Pulse.glsl")->
                getOrCreateUniform("bIsOut",osg::Uniform::BOOL)->set(T::m_bOut);*/}
    void FreqChanged(){/*T::m_pSceneGraph->ResouceLoader()->GetOrCreateStateSet("GLSL/Pulse.glsl")->
                getOrCreateUniform("fPulseIntervalTime",osg::Uniform::FLOAT)->set(1.f/T::m_unFreq);*/}

    void FrameCall()
    {
        if(m_bDistanceChanged)
        {
            m_pScalTransform->setMatrix(osg::Matrix::scale(T::m_dDistance,T::m_dDistance,T::m_dDistance));
            m_bDistanceChanged=false;
        }

        if(m_bShowTypeChanged)
        {
            if(T::m_bShowLine)
            {
                m_pLineGroup->setNodeMask(0xffffffffu);
            }
            else
            {
                m_pLineGroup->setNodeMask(0);
            }

            if(T::m_bShowFace)
            {
                m_pFaceGroup->setNodeMask(0xffffffffu);
            }
            else
            {
                m_pFaceGroup->setNodeMask(0);
            }
            m_bShowTypeChanged=false;
        }

        if(m_bEffectChanged)
        {
            switch(T::m_emEffect)
            {
            case T::UNIFORM_MOTION:
                ImplSceneShape<T>::m_unStateSet |= PULSE_DRAW_STATE;
                break;
            default:
                ImplSceneShape<T>::m_unStateSet &= ~PULSE_DRAW_STATE;
                break;
            }


            auto pStateSet = T::m_pSceneGraph->ResouceLoader()->GetOrCreateStateSet(ImplSceneShape<T>::m_unStateSet);
            m_pProgramNode->setStateSet(pStateSet);
            m_bEffectChanged=false;
        }

        if(m_bShowBackChanged)
        {
            if(!T::m_bShowBack)
            {
//                /// 开启背面裁剪
//                if(!m_pCullFace.valid())
//                {
//                    m_pCullFace = new osg::CullFace;
//                    auto pNodeStateSet = ImplSceneShape<T>::m_pGeometry->getOrCreateStateSet();
//                    pNodeStateSet->setAttributeAndModes(m_pCullFace);
//                }
//            }
//            else
//            {
//                auto pStateSet = ImplSceneShape<T>::m_pGeometry->getStateSet();
//                if(nullptr != pStateSet)
//                {
//                    pStateSet->removeAssociatedModes(m_pCullFace);
//                }
            }
            m_bShowBackChanged=false;
        }

        ImplSceneShape<T>::FrameCall();
    }

    /// 重写初始化状态
    void InitNode()
    {
        ImplSceneShape<T>::InitNode();
        ImplSceneShape<T>::m_pGeometry->setStateSet(ImplSceneShape<T>::m_pSceneGraph->ResouceLoader()->GetOrCreateStateSet(BLEND_STATE));

        m_pLineGroup = new osg::Group;
        m_pFaceGroup = new osg::Group;
        m_pRenderGroup = new osg::Group;
        m_pScalTransform = new osg::MatrixTransform;

        /// 设置绑定的属性
        CountChanged();
        DirectionChanged();
        FreqChanged();

        m_pScalTransform->setMatrix(osg::Matrix::scale(T::m_dDistance,T::m_dDistance,T::m_dDistance));
        m_pScalTransform->addChild(m_pLineGroup.get());
        m_pScalTransform->addChild(m_pFaceGroup.get());

        m_pRenderGroup->addChild(ImplSceneShape<T>::m_pGeometry.get());
        m_pRenderGroup->addChild(ImplSceneShape<T>::m_pGeometry.get());

        m_pLineGroup->addChild(m_pRenderGroup);
        m_pFaceGroup->addChild(m_pRenderGroup);

        /// 线模型只绘制线 面模型只绘制面
        m_pRenderGroup->setStateSet(ImplSceneShape<T>::m_pSceneGraph->ResouceLoader()->GetOrCreateStateSet(BLEND_STATE));
        m_pLineGroup->setStateSet(ImplSceneShape<T>::m_pSceneGraph->ResouceLoader()->GetOrCreateStateSet(LINE_STATE));
        m_pFaceGroup->setStateSet(ImplSceneShape<T>::m_pSceneGraph->ResouceLoader()->GetOrCreateStateSet(FACE_STATE));

        ImplSceneShape<T>::SetOsgNode(m_pScalTransform.get());
    }
protected:
    osg::observer_ptr<osg::Group>    m_pFaceGroup;
    osg::observer_ptr<osg::Group>    m_pLineGroup;
    osg::ref_ptr<osg::Group>         m_pRenderGroup;
    osg::observer_ptr<osg::MatrixTransform> m_pScalTransform;
    bool       m_bDistanceChanged{false};
    bool       m_bShowTypeChanged{false};
    bool       m_bEffectChanged{false};
    bool       m_bShowBackChanged{false};
};

#endif // IMPL_SCENE_SENSOR_H
