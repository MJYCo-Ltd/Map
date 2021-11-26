#include "FlatAbility.h"
#include <Inner/ILoadResource.h>

void CFlatAbility::InitAbility()
{
    m_pSceneNode->MergeStateSet(FLAT_STATE);
}

void CFlatAbility::UpdateAbility()
{
    ImplNodeAbility<IFlatAbility>::UpdateAbility();
}

int CFlatAbility::AddHole(const FlatArea &flatArea)
{
    if(m_mapId2Hole.size() > 9)
    {
        return(-1);
    }
    m_vInsertHole.push_back(flatArea);
    m_pSceneNode->AbilityChanged();
}

bool CFlatAbility::RemoveHole(int nIndex)
{
    return(true);
}
