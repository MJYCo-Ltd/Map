#ifndef IMPL_SCENE_SCREEN_GROUP_H
#define IMPL_SCENE_SCREEN_GROUP_H

#include <osgEarth/ScreenSpaceLayout>
#include <Inner/Common/ImplSceneGroup.hpp>

/**
 *  实现ISceneScreenGroup的接口
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
        T::m_bOpenPreCut=false;
        ImplSceneGroup<T>::InitNode();
        osgEarth::ScreenSpaceLayout::activate(IOsgSceneNode::m_pRootNode->getOrCreateStateSet());
        osgEarth::ScreenSpaceLayout::setDeclutteringEnabled(false);
    }

    void AvoidChanged()
    {
        if(T::m_bAvoid)
        {
            osgEarth::ScreenSpaceLayout::setDeclutteringEnabled(true);
        }
        else
        {
            osgEarth::ScreenSpaceLayout::setDeclutteringEnabled(false);
        }
    }
};

#endif // IMPL_SCENE_FLASH_GROUP_H
