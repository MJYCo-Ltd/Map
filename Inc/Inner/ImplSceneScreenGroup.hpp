#ifndef IMPL_SCENE_SCREEN_GROUP_H
#define IMPL_SCENE_SCREEN_GROUP_H

#include <osgEarth/ScreenSpaceLayout>
#include <Inner/ImplSceneGroup.hpp>

/**
 *  实现ISceneNode所有的接口
 */
template <typename T>
class ImplSceneScreenGroup:public ImplSceneGroup<T>
{
public:
    CONSTRUCTOR(ImplSceneScreenGroup,ImplSceneGroup<T>)

protected:

    /**
     * @brief 初始化场景节点
     */
    void InitNode()
    {
        ImplSceneGroup<T>::InitNode();
        osgEarth::ScreenSpaceLayout::activate(IOsgSceneNode::m_pRootNode->getOrCreateStateSet());
    }
};

#endif // IMPL_SCENE_FLASH_GROUP_H
