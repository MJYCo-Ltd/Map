#ifndef IMPL_SCENE_LOD_GROUP_H
#define IMPL_SCENE_LOD_GROUP_H

#include <osg/LOD>
#include <SceneGraph/ISceneGroup.h>
#include <Inner/Common/ImplSceneGroup.hpp>


/**
 *  实现ISceneLodGroup所有的接口
 */
template <typename T>
class ImplSceneLodGroup:public ImplSceneGroup<T>
{
public:
    CONSTRUCTOR(ImplSceneLodGroup,ImplSceneGroup<T>)
	
protected:
    void InitNode()
    {
        ImplSceneNode<T>::InitNode();
        m_pLod = new osg::LOD;
        ImplSceneGroup<T>::SetGroupNode(m_pLod.get());
    }

    void ChangeTypeChanged()SET_TRUE_NODE_UPDATE(m_bTypeChanged)
    void LevelsChanged()SET_TRUE_NODE_UPDATE(m_bLevelsChanged)

    void UpdateNode()
    {
        if(m_bTypeChanged)
        {
            switch (T::m_emType)
            {
            case T::NODE_EYE_DISTANCE:
                m_pLod->setRangeMode(osg::LOD::DISTANCE_FROM_EYE_POINT);
                break;
            case T::NODE_SCREEN_PIXEL:
                m_pLod->setRangeMode(osg::LOD::PIXEL_SIZE_ON_SCREEN);
                break;
            }
            m_bTypeChanged =false;
        }

        if(m_bLevelsChanged)
        {
            if(m_pLod->getNumChildren()-1 == T::m_vLevelInfos.size())
            {
                int i=0;
                m_pLod->setRange(i,T::m_vLevelInfos[i],FLT_MAX);

                for(i=1; i<T::m_vLevelInfos.size();++i)
                {
                    m_pLod->setRange(i,T::m_vLevelInfos[i],T::m_vLevelInfos[i-1]);
                }

                m_pLod->setRange(i,0,T::m_vLevelInfos[i-1]);
            }
            m_bTypeChanged = false;
        }
        ImplSceneGroup<T>::UpdateNode();
    }

protected:
    osg::observer_ptr<osg::LOD> m_pLod;
    bool  m_bTypeChanged=false;
    bool  m_bLevelsChanged=false;
};

#endif // QT_OSG_SCENE_NODE_H
