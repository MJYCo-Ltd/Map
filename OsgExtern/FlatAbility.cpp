#include "FlatAbility.h"
#include <Inner/ILoadResource.h>

void CFlatAbility::InitAbility()
{
    m_pSceneNode->MergeStateSet(FLAT_STATE);

    m_pHoleMatrix = new osg::Uniform(osg::Uniform::FLOAT_MAT4x2,"inVertex",10);
    m_pHoleNum = new osg::Uniform(osg::Uniform::INT,"polySize");

    /// 增加
    m_pSceneNode->AsOsgSceneNode()->GetOsgNode()->getOrCreateStateSet()->addUniform(m_pHoleMatrix);
    m_pSceneNode->AsOsgSceneNode()->GetOsgNode()->getOrCreateStateSet()->addUniform(m_pHoleNum);
}

void CFlatAbility::UpdateAbility()
{
    ImplNodeAbility<IFlatAbility>::UpdateAbility();
    m_pHoleNum->set(m_mapId2Hole.size());

    int nIndex=0;
    osg::Matrix4x2 tmpMat;
    for(auto one=m_mapId2Hole.begin();one!=m_mapId2Hole.end();++one,++nIndex)
    {
        const FlatArea& rFlatArea = one->second;
        tmpMat.set(rFlatArea.stLeftUp.dX,rFlatArea.stLeftUp.dY,
                   rFlatArea.stRightUp.dX,rFlatArea.stRightUp.dY,
                   rFlatArea.stRightDown.dX,rFlatArea.stRightDown.dY,
                   rFlatArea.stLeftDown.dX,rFlatArea.stLeftDown.dY);
        m_pHoleMatrix->setElement(nIndex,tmpMat);
    }
}

/// 增加洞
int CFlatAbility::AddHole(const FlatArea &flatArea)
{
    if(m_mapId2Hole.size() > 9)
    {
        return(-1);
    }

    /// 插入值
    for(int nIndex=0;nIndex<10;++nIndex)
    {
        if(m_mapId2Hole.end() == m_mapId2Hole.find(nIndex))
        {
            m_mapId2Hole[nIndex] = flatArea;
            break;
        }
    }

    m_pSceneNode->AbilityChanged();
}

bool CFlatAbility::RemoveHole(int nIndex)
{
    auto findOne = m_mapId2Hole.find(nIndex);

    if(m_mapId2Hole.end() == findOne)
    {
        return(false);
    }
    else
    {
        m_mapId2Hole.erase(findOne);
    }

    m_pSceneNode->AbilityChanged();
    return(true);
}
