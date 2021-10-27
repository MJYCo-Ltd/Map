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
        ImplSceneGroup<T>::m_pProgramNode->setStateSet(
                    T::m_pSceneGraph->ResouceLoader()->GetOrCreateStateSet("GLSL/Flash.glsl"));
    }

    void FlashStatusChanged()SET_TRUE_NODE_UPDATE(m_bStatusChanged)
    void FlashColorChanged()SET_TRUE_NODE_UPDATE(m_bColorChanged)
    void FlashFlashChanged()SET_TRUE_NODE_UPDATE(m_bFlashChanged)

    void FrameCall()
    {
        if(m_bFlashChanged)
        {
            ImplSceneGroup<T>::m_pRootNode->getOrCreateStateSet()->
                    getOrCreateUniform("openFlag",osg::Uniform::BOOL)->set(T::m_bFlash);
            m_bFlashChanged = false;
        }

        if(m_bColorChanged)
        {
            ImplSceneGroup<T>::m_pRootNode->getOrCreateStateSet()->
                getOrCreateUniform("flashColor",osg::Uniform::FLOAT_VEC4)->
                    set(osg::Vec4(T::m_stFlahColor.fR,T::m_stFlahColor.fG,T::m_stFlahColor.fB,T::m_stFlahColor.fA));
            m_bColorChanged=false;
        }

        if(m_bStatusChanged)
        {
            auto pState = ImplSceneGroup<T>::m_pRootNode->getOrCreateStateSet();
            pState->getOrCreateUniform("flashStartTime",osg::Uniform::FLOAT)->set((float)osg::Timer::instance()->time_s());
            pState->getOrCreateUniform("flashIntervalTime",osg::Uniform::FLOAT)->set(1.f/T::m_fFlashHZ);
            pState->getOrCreateUniform("flashDurTime",osg::Uniform::FLOAT)->set(0.5f/T::m_fFlashHZ);
            m_bStatusChanged=false;
        }

        ImplSceneGroup<T>::FrameCall();
    }
protected:
    bool m_bStatusChanged{false};
    bool m_bColorChanged{false};
    bool m_bFlashChanged{false};
};

#endif // IMPL_SCENE_FLASH_GROUP_H
