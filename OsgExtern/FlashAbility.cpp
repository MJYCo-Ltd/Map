#include <SceneGraph/ISceneGraph.h>
#include <Inner/ILoadResource.h>
#include "FlashAbility.h"

void CFlashAbility::InitAbility()
{
    /// 设置着色器
    auto pSate = m_pSceneNode->AsOsgSceneNode()->GetOsgNode()->getOrCreateStateSet();
    m_pVirutlProgram = osgEarth::VirtualProgram::getOrCreate(pSate);

    /// 此处应该不知道
    if(m_pSceneNode->GetBoundSceneGraph()->ResouceLoader()->LoadVirtualProgram(m_pVirutlProgram,"GLSL/Flash.glsl"))
    {
        /// 获取闪烁变量
        m_pFlashStartTime = pSate->getOrCreateUniform("flashStartTime",osg::Uniform::FLOAT);
        m_pFlashDurTime = pSate->getOrCreateUniform("flashDurTime",osg::Uniform::FLOAT);
        m_pFlashIntervalTime = pSate->getOrCreateUniform("flashIntervalTime",osg::Uniform::FLOAT);
        m_pFlashColor = pSate->getOrCreateUniform("flashColor",osg::Uniform::FLOAT_VEC4);

        /// 设置闪烁变量
        m_pFlashStartTime->set((float)osg::Timer::instance()->time_s());
        m_pFlashDurTime->set(.5f/m_nFlashHZ);
        m_pFlashIntervalTime->set(1.f/m_nFlashHZ);
        m_pFlashColor->set(osg::Vec4(m_stFlahColor.fR,m_stFlahColor.fG,m_stFlahColor.fB,m_stFlahColor.fA));
    }
}

void CFlashAbility::UpdateAbility()
{
    ImplNodeAbility<IFlashAbility>::UpdateAbility();

    /// 闪射状态更改
    if(m_bStatusChanged)
    {
        int nFlashHZ=m_nFlashHZ;

        if(nFlashHZ < 1)
        {
            nFlashHZ = -1;
        }

        if(nFlashHZ > 60)
        {
            nFlashHZ = 60;
        }

        /// 更新数据
        m_pFlashDurTime->set(.5f/nFlashHZ);
        m_pFlashIntervalTime->set(1.f/nFlashHZ);

        m_bStatusChanged=false;
    }

    /// 闪烁颜色更改
    if(m_bColorChanged)
    {
        m_pFlashColor->set(osg::Vec4(m_stFlahColor.fR,m_stFlahColor.fG,m_stFlahColor.fB,m_stFlahColor.fA));

        m_bColorChanged=false;
    }

    ///闪烁状态更改
    if(m_bFlashChanged)
    {
        if(m_bFlash)
        {
            m_pSceneNode->GetBoundSceneGraph()->ResouceLoader()->LoadVirtualProgram(m_pVirutlProgram,"GLSL/Flash.glsl");
        }
        else
        {
            m_pSceneNode->GetBoundSceneGraph()->ResouceLoader()->RemoveVirtualProgram(m_pVirutlProgram,"GLSL/Flash.glsl");
        }
        m_bFlashChanged=false;
    }
}
