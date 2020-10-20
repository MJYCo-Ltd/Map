#ifndef IMPL_SCENE_SCALE_GROUP_H
#define IMPL_SCENE_SCALE_GROUP_H

#include <osg/AutoTransform>
#include "ImplSceneGroup.hpp"

/**
 *  实现ISceneScaleGroup所有的接口
 */
template <typename T>
class ImplSceneScaleGroup:public ImplSceneGroup<T>
{
public:
    CONSTRUCTOR(ImplSceneScaleGroup,ImplSceneGroup<T>)

protected:
    void InitNode()
    {
        m_pAutoScaleTransform = new osg::AutoTransform;
        m_pAutoScaleTransform->setAutoScaleToScreen(true);
        SetOsgNode(m_pAutoScaleTransform);

        ImplSceneGroup<T>::InitNode();
    }

    /**
     * @brief 位置更改
     */
    void ScalChanged()
    {
    }

    void AutoScalChanged()
    {
    }

protected:
    osg::AutoTransform* m_pAutoScaleTransform;
};

#endif // IMPL_SCENE_MODEL_H
