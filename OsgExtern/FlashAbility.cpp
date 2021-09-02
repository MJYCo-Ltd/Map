#include <SceneGraph/ISceneGraph.h>
#include <Inner/ILoadResource.h>
#include "FlashAbility.h"

void CFlashAbility::InitAbility()
{
    m_pFlashStateSet = m_pSceneNode->GetBoundSceneGraph()->ResouceLoader()->LoadVirtualProgram("GLSL/Flash.glsl");
    /// 如果节点下有着色器
    if(m_pSceneNode->AsOsgSceneNode()->GetOsgNode()->getStateSet())
    {
        m_pSceneNode->AsOsgSceneNode()->GetOsgNode()->setStateSet(
                    m_pSceneNode->GetBoundSceneGraph()->ResouceLoader()->MergeStateSet(
                        m_pFlashStateSet.get(),
                        m_pSceneNode->AsOsgSceneNode()->GetOsgNode()->getStateSet()));
    }
    else
    {
        m_pSceneNode->AsOsgSceneNode()->GetOsgNode()->setStateSet(m_pFlashStateSet.get());
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
//        m_pFlashDurTime->set(.5f/nFlashHZ);
//        m_pFlashIntervalTime->set(1.f/nFlashHZ);

        m_bStatusChanged=false;
    }

    /// 闪烁颜色更改
    if(m_bColorChanged)
    {
//        m_pFlashColor->set(osg::Vec4(m_stFlahColor.fR,m_stFlahColor.fG,m_stFlahColor.fB,m_stFlahColor.fA));

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
           m_pSceneNode->GetBoundSceneGraph()->ResouceLoader()->RemoveVirtualProgram("GLSL/Flash.glsl",
                                                                                     m_pSceneNode->AsOsgSceneNode()->GetOsgNode()->getStateSet());
        }
        m_bFlashChanged=false;
    }
}
