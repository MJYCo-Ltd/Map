#ifndef CFLASHABILITY_H
#define CFLASHABILITY_H
#include <osg/Group>
#include <osgEarth/VirtualProgram>
#include <Ability/IFlashAbility.h>
#include "ImplNodeAbility.hpp"

class CFlashAbility : public ImplNodeAbility<IFlashAbility>
{
public:
    /**
     * @brief 初始化能力
     */
    void InitAbility();

    /**
     * @brief 移除能力
     */
    void RemoveAbility();

    /**
     * @brief 更新能力
     */
    void UpdateAbility();
protected:
    void FlashStatusChanged()SET_TRUE_ABILITY_UPDATE(m_bStatusChanged)
    void FlashColorChanged()SET_TRUE_ABILITY_UPDATE(m_bColorChanged)
    void FlashFlashChanged()SET_TRUE_ABILITY_UPDATE(m_bFlashChanged)
protected:
    osg::ref_ptr<osg::Uniform>             m_pFlashStartTime;
    osg::ref_ptr<osg::Uniform>             m_pFlashDurTime;
    osg::ref_ptr<osg::Uniform>             m_pFlashIntervalTime;
    osg::ref_ptr<osg::Uniform>             m_pFlashColor;
    osg::ref_ptr<osg::StateSet>            m_pOldStateSet;
    osg::observer_ptr<osg::StateSet>       m_pFlashStateSet;
    bool m_bStatusChanged{false};
    bool m_bColorChanged{false};
    bool m_bFlashChanged{false};
};

#endif // CFLASHABILITY_H
