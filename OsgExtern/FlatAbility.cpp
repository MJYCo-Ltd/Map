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

int CFlatAbility::AddHole(const ScenePos &leftUp, const ScenePos &rightDown)
{
    return(-1);
}

bool CFlatAbility::RemoveHole(int nIndex)
{
    return(true);
}
