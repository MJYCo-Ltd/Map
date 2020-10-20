#ifndef IMPL_SCENE_LOD_GROUP_H
#define IMPL_SCENE_LOD_GROUP_H

#include <osg/LOD>
#include <SceneGraph/ISceneGroup.h>
#include <Inner/ImplSceneGroup.hpp>


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
        m_pLod = new osg::LOD;
        SetOsgNode(m_pLod);
    }

    void ChangeTypeChanged(){}
    void LevelsChanged(){}

protected:
    osg::LOD* m_pLod;
};

#endif // QT_OSG_SCENE_NODE_H
