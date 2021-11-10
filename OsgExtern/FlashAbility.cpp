#include <SceneGraph/ISceneGraph.h>
#include <Inner/ILoadResource.h>
#include "FlashAbility.h"

void CFlashAbility::InitAbility()
{
    m_pSceneNode->AsOsgSceneNode()->GetRealNode()->setStateSet(
                m_pSceneNode->GetBoundSceneGraph()->ResouceLoader()->GetOrCreateStateSet(
                    m_pSceneNode->AsOsgSceneNode()->GetStateSet()|FLASH_DRAW_STATE));

    m_pSceneNode->AsOsgSceneNode()->GetOsgNode()->getOrCreateStateSet()
            ->getOrCreateUniform("flashStartTime",osg::Uniform::FLOAT)->set((float)osg::Timer::instance()->time_s());
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
        m_pSceneNode->AsOsgSceneNode()->GetOsgNode()->getOrCreateStateSet()
                ->getOrCreateUniform("flashDurTime",osg::Uniform::FLOAT)->set(.5f/nFlashHZ);
        m_pSceneNode->AsOsgSceneNode()->GetOsgNode()->getOrCreateStateSet()
                ->getOrCreateUniform("flashIntervalTime",osg::Uniform::FLOAT)->set(1.f/nFlashHZ);

        m_bStatusChanged=false;
    }

    /// 闪烁颜色更改
    if(m_bColorChanged)
    {
        m_pSceneNode->AsOsgSceneNode()->GetOsgNode()->getOrCreateStateSet()
                ->getOrCreateUniform("flashColor",osg::Uniform::FLOAT_VEC4)
                ->set(osg::Vec4(m_stFlahColor.fR,m_stFlahColor.fG,m_stFlahColor.fB,m_stFlahColor.fA));

        m_bColorChanged=false;
    }

    ///闪烁状态更改
    if(m_bFlashChanged)
    {
        if(m_bFlash)
        {
            InitAbility();
        }
        else
        {
            m_pSceneNode->AsOsgSceneNode()->GetRealNode()->setStateSet(nullptr);
        }
        m_bFlashChanged=false;
    }
}
