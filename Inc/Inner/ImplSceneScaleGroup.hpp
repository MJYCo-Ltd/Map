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
        ImplSceneNode<T>::InitNode();
        m_pAutoScaleTransform = new osg::AutoTransform;
        m_pAutoScaleTransform->setAutoScaleToScreen(true);
        ImplSceneGroup<T>::SetGroupNode(m_pAutoScaleTransform.get());
    }

    /**
     * @brief 位置更改
     */
    void ScalChanged()
    {
        m_bScaleChanged = true;
        ImplSceneGroup<T>::NodeChanged();
    }

    void AutoScalChanged()
    {
    }

    void UpdateNode()
    {
        if(m_bScaleChanged)
        {
            //m_pAutoScaleTransform->setScale(T::m_dScalBit);
            m_pAutoScaleTransform->setMinimumScale(T::m_dScalBit);
            m_bScaleChanged = false;
        }
        ImplSceneGroup<T>::UpdateNode();
    }

protected:
    osg::observer_ptr<osg::AutoTransform> m_pAutoScaleTransform;
    bool m_bScaleChanged=false;
};

#endif // IMPL_SCENE_MODEL_H
