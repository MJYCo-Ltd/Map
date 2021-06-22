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
    void CountChanged(){m_pPulseStep->set(1.f/T::m_unCount);}
    void DirectionChanged(){m_pbIsOut->set(T::m_bOut);}
    void FreqChanged(){m_pPulseIntervalTime->set(1.f/T::m_unFreq);}

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
            if(!m_sCurrentVirtulProgram.empty())
            {
                T::m_pSceneGraph->ResouceLoader()->RemoveVirtualProgram(m_pVirutlProgram,m_sCurrentVirtulProgram);
            }

            switch(T::m_emEffect)
            {
            case T::UNIFORM_MOTION:
                m_sCurrentVirtulProgram="GLSL/Pulse.glsl";
                break;
            default:
                m_sCurrentVirtulProgram="";
                break;
            }

            if(!m_sCurrentVirtulProgram.empty())
            {
                T::m_pSceneGraph->ResouceLoader()->LoadVirtualProgram(m_pVirutlProgram,m_sCurrentVirtulProgram);
            }
            m_bEffectChanged=false;
        }

        if(m_bShowBackChanged)
        {
            if(!T::m_bShowBack)
            {
                /// 开启背面裁剪
                m_pCullFace = new osg::CullFace;
                m_pGeometry->getOrCreateStateSet()->setAttributeAndModes(m_pCullFace);
            }
            else
            {
                m_pGeometry->getOrCreateStateSet()->removeAssociatedModes(m_pCullFace);
            }
            m_bShowBackChanged=false;
        }

        ImplSceneShape<T>::FrameCall();
    }

    /// 重写初始化状态
    void InitNode()
    {
        ImplSceneShape<T>::InitNode();

        m_pLineGroup = new osg::Group;
        m_pFaceGroup = new osg::Group;
        m_pScalTransform = new osg::MatrixTransform;

        auto pSate = m_pScalTransform->getOrCreateStateSet();
        m_pVirutlProgram = osgEarth::VirtualProgram::getOrCreate(pSate);
        m_pPulseStartTime = pSate->getOrCreateUniform("fPulseStartTime",osg::Uniform::FLOAT);
        m_pPulseIntervalTime = pSate->getOrCreateUniform("fPulseIntervalTime",osg::Uniform::FLOAT);
        m_pbIsOut=pSate->getOrCreateUniform("bIsOut",osg::Uniform::BOOL);
        m_pPulseStep=pSate->getOrCreateUniform("fPulseStep",osg::Uniform::FLOAT);

        /// 设置绑定的属性
        m_pPulseStartTime->set((float)osg::Timer::instance()->time_s());
        CountChanged();
        DirectionChanged();
        FreqChanged();

        m_pScalTransform->setMatrix(osg::Matrix::scale(T::m_dDistance,T::m_dDistance,T::m_dDistance));
        m_pScalTransform->addChild(m_pLineGroup.get());
        m_pScalTransform->addChild(m_pFaceGroup.get());

        m_pLineGroup->addChild(ImplSceneShape<T>::m_pGeometry.get());
        m_pFaceGroup->addChild(ImplSceneShape<T>::m_pGeometry.get());

        /// 线模型只绘制线 面模型只绘制面
        m_pLineGroup->getOrCreateStateSet()->setAttributeAndModes(new osg::PolygonMode
                                                                  (osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE));
        m_pFaceGroup->getOrCreateStateSet()->setAttributeAndModes(new osg::PolygonMode
                                                                  (osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::FILL));

        ImplSceneShape<T>::SetOsgNode(m_pScalTransform.get());
    }
protected:
    osg::observer_ptr<osg::Group>    m_pFaceGroup;
    osg::observer_ptr<osg::Group>    m_pLineGroup;
    osg::observer_ptr<osg::MatrixTransform> m_pScalTransform;
    osg::ref_ptr<osgEarth::VirtualProgram> m_pVirutlProgram;
    osg::ref_ptr<osg::CullFace>            m_pCullFace;
    osg::ref_ptr<osg::Uniform>             m_pPulseStartTime;
    osg::ref_ptr<osg::Uniform>             m_pPulseIntervalTime;
    osg::ref_ptr<osg::Uniform>             m_pbIsOut;
    osg::ref_ptr<osg::Uniform>             m_pPulseStep;
    std::string m_sCurrentVirtulProgram;
    bool       m_bDistanceChanged{false};
    bool       m_bShowTypeChanged{false};
    bool       m_bEffectChanged{false};
    bool       m_bShowBackChanged{false};
};

#endif // IMPL_SCENE_SENSOR_H
