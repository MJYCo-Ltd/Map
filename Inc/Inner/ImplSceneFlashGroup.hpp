#ifndef IMPL_SCENE_FLASH_GROUP_H
#define IMPL_SCENE_FLASH_GROUP_H

#include <osg/Timer>
#include <osgEarth/VirtualProgram>
#include <osgEarth/Shaders>

#include <Inner/ImplSceneGroup.hpp>
#include <SceneGraph/ISceneGroup.h>
#include <Inner/ILoadResource.h>

/**
 *  实现ISceneNode所有的接口
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

        {
            /// 设置着色器
            auto pSate = ImplSceneGroup<T>::m_pRootNode->getOrCreateStateSet();
            m_pVirutlProgram = osgEarth::VirtualProgram::getOrCreate(pSate);

            T::m_pSceneGraph->ResouceLoader()->LoadVirtualProgram(m_pVirutlProgram,"Data/GLSL/Global.glsl");

            /// 此处应该不知道
            if(T::m_pSceneGraph->ResouceLoader()->LoadVirtualProgram(m_pVirutlProgram,"Data/GLSL/Flash.glsl"))
            {
                /// 获取闪烁变量
                m_pFlashStartTime = pSate->getOrCreateUniform("flashStartTime",osg::Uniform::FLOAT);
                m_pFlashDurTime = pSate->getOrCreateUniform("flashDurTime",osg::Uniform::FLOAT);
                m_pFlashIntervalTime = pSate->getOrCreateUniform("flashIntervalTime",osg::Uniform::FLOAT);
                m_pFlashColor = pSate->getOrCreateUniform("flashColor",osg::Uniform::FLOAT_VEC4);

                /// 设置闪烁变量
                m_pFlashStartTime->set((float)osg::Timer::instance()->time_s());
                m_pFlashDurTime->set(0.5f/T::m_fFlashHZ);
                m_pFlashIntervalTime->set(1.f/T::m_fFlashHZ);
                m_pFlashColor->set(osg::Vec4(T::m_stFlahColor.fR,T::m_stFlahColor.fG,T::m_stFlahColor.fB,T::m_stFlahColor.fA));
            }
        }
    }

    void FlashStatusChanged()
    {
        T::m_bStatusChanged=true;
        ImplSceneGroup<T>::NodeChanged();
    }

    void FlashColorChanged()
    {
        T::m_bColorChanged=true;
        ImplSceneGroup<T>::NodeChanged();
    }

    void FlashFlashChanged()
    {
        T::m_bFlashChanged=true;
        ImplSceneGroup<T>::NodeChanged();
    }

    void UpdateNode()
    {
        if(T::m_bFlashChanged)
        {
            if(!T::m_bFlash)
            {
                T::m_pSceneGraph->ResouceLoader()->RemoveVirtualProgram(m_pVirutlProgram,"Data/GLSL/Flash.glsl");
            }
            else
            {
                T::m_pSceneGraph->ResouceLoader()->LoadVirtualProgram(m_pVirutlProgram,"Data/GLSL/Flash.glsl");
            }
            T::m_bFlashChanged = false;
        }

        if(T::m_bColorChanged)
        {
            if(m_pFlashColor.valid())
            {
                m_pFlashColor->set(osg::Vec4(T::m_stFlahColor.fR,T::m_stFlahColor.fG,T::m_stFlahColor.fB,T::m_stFlahColor.fA));
            }
            T::m_bColorChanged=false;
        }

        if(T::m_bStatusChanged)
        {
            if(m_pFlashIntervalTime.valid())
            {
                m_pFlashIntervalTime->set(1.f/T::m_fFlashHZ);
                m_pFlashDurTime->set(0.5f/T::m_fFlashHZ);
            }


            T::m_bStatusChanged=false;
        }
        ImplSceneGroup<T>::UpdateNode();
    }
protected:
    osg::ref_ptr<osg::Uniform>             m_pFlashStartTime;
    osg::ref_ptr<osg::Uniform>             m_pFlashDurTime;
    osg::ref_ptr<osg::Uniform>             m_pFlashIntervalTime;
    osg::ref_ptr<osg::Uniform>             m_pFlashColor;
    osg::ref_ptr<osgEarth::VirtualProgram> m_pVirutlProgram;
};

#endif // IMPL_SCENE_FLASH_GROUP_H
