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
        if(!s_pStateSet.valid())
        {
            s_pStateSet = new osg::StateSet;
            osgEarth::ScreenSpaceLayout::activate(s_pStateSet);
        }

        T::m_bOpenCull=false;
        ImplSceneGroup<T>::InitNode();
        ImplSceneGroup<T>::m_pGroup->setStateSet(s_pStateSet);
        osgEarth::ScreenSpaceLayout::setDeclutteringEnabled(false);
        auto options = osgEarth::ScreenSpaceLayout::getOptions();
        options.technique() = options.TECHNIQUE_CALLOUTS;
        osgEarth::ScreenSpaceLayout::setOptions(options);
        IOsgSceneNode::m_pRootNode->setCullingActive(false);
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
    static osg::ref_ptr<osg::StateSet> s_pStateSet;
};

template <typename T>
osg::ref_ptr<osg::StateSet> ImplSceneScreenGroup<T>::s_pStateSet;
#endif // IMPL_SCENE_FLASH_GROUP_H
