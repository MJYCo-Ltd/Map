#include<Inner/OsgExtern/NodeAbilityManager.h>
#include "FlashAbility.h"

CNodeAbilityManager *CNodeAbilityManager::GetInstance()
{
    static CNodeAbilityManager s_gNodeAbilityManager;
    return(&s_gNodeAbilityManager);
}

INodeAbility *CNodeAbilityManager::CreateAbility(ABILITY_TYPE emType)
{
    switch (emType)
    {
    case FLASH_ABILITY:
        return(new CFlashAbility);
    case AUTOSCAL_ABILITY:
        break;
    case MOTION_ABILITY:
        break;
    default:
        break;
    }
    return(nullptr);
}

bool CNodeAbilityManager::RemoveAbility(INodeAbility *&pNodeAbility)
{
    pNodeAbility = nullptr;
    return(true);
}
