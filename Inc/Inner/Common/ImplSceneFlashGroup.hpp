#ifndef IMPL_SCENE_FLASH_GROUP_H
#define IMPL_SCENE_FLASH_GROUP_H

#include <osg/Timer>
#include <osgEarth/VirtualProgram>
#include <osgEarth/Shaders>

#include <Inner/Common/ImplSceneGroup.hpp>
#include <SceneGraph/ISceneGroup.h>
#include <Inner/ILoadResource.h>

/**
 *  实现ISceneFlashGroup的接口
 */
template <typename T>
class ImplSceneFlashGroup:public ImplSceneGroup<T>
{
public:
    CONSTRUCTOR(ImplSceneFlashGroup,ImplSceneGroup<T>)

protected:

    /**
     * @brief 初始化场景节点
     */
    void InitNode()
    {
        ImplSceneGroup<T>::InitNode();
        m_pProgramNode->setStateSet(T::m_pSceneGraph->ResouceLoader()->CreateStateSet("GLSL/Flash.glsl"));

        /// 设置着色器
        auto pSate = ImplSceneGroup<T>::m_pRootNode->getOrCreateStateSet();
        {

            /// 获取闪烁变量
            m_pFlashStartTime = pSate->getOrCreateUniform("flashStartTime",osg::Uniform::FLOAT);
            m_pFlashDurTime = pSate->getOrCreateUniform("flashDurTime",osg::Uniform::FLOAT);
            m_pFlashIntervalTime = pSate->getOrCreateUniform("flashIntervalTime",osg::Uniform::FLOAT);
            m_pFlashColor = pSate->getOrCreateUniform("flashColor",osg::Uniform::FLOAT_VEC4);
            m_pOpenFlag = pSate->getOrCreateUniform("openFlag",osg::Uniform::BOOL);


            /// 设置闪烁变量
            m_pFlashStartTime->set((float)osg::Timer::instance()->time_s());
            m_pFlashDurTime->set(0.5f/T::m_fFlashHZ);
            m_pFlashIntervalTime->set(1.f/T::m_fFlashHZ);
            m_pFlashColor->set(osg::Vec4(T::m_stFlahColor.fR,T::m_stFlahColor.fG,T::m_stFlahColor.fB,T::m_stFlahColor.fA));
            m_pOpenFlag->set(T::m_bFlash);
        }
    }

    void FlashStatusChanged()SET_TRUE_NODE_UPDATE(m_bStatusChanged)
    void FlashColorChanged()SET_TRUE_NODE_UPDATE(m_bColorChanged)
    void FlashFlashChanged()SET_TRUE_NODE_UPDATE(m_bFlashChanged)

    void FrameCall()
    {
        if(m_bFlashChanged)
        {
            m_pOpenFlag->set(T::m_bFlash);
            m_bFlashChanged = false;
        }

        if(m_bColorChanged)
        {
            if(m_pFlashColor.valid())
            {
                m_pFlashColor->set(osg::Vec4(T::m_stFlahColor.fR,T::m_stFlahColor.fG,T::m_stFlahColor.fB,T::m_stFlahColor.fA));
            }
            m_bColorChanged=false;
        }

        if(m_bStatusChanged)
        {
            if(m_pFlashIntervalTime.valid())
            {
                m_pFlashIntervalTime->set(1.f/T::m_fFlashHZ);
                m_pFlashDurTime->set(0.5f/T::m_fFlashHZ);
            }


            m_bStatusChanged=false;
        }
        ImplSceneGroup<T>::FrameCall();
    }
protected:
    osg::ref_ptr<osg::Uniform>             m_pFlashStartTime;
    osg::ref_ptr<osg::Uniform>             m_pFlashDurTime;
    osg::ref_ptr<osg::Uniform>             m_pFlashIntervalTime;
    osg::ref_ptr<osg::Uniform>             m_pFlashColor;
    osg::ref_ptr<osg::Uniform>             m_pOpenFlag;
    osg::ref_ptr<osgEarth::VirtualProgram> m_pVirutlProgram;
    bool m_bStatusChanged{false};
    bool m_bColorChanged{false};
    bool m_bFlashChanged{false};
};

#endif // IMPL_SCENE_FLASH_GROUP_H
