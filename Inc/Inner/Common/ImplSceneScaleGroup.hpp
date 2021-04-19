#ifndef IMPL_SCENE_SCALE_GROUP_H
#define IMPL_SCENE_SCALE_GROUP_H

#include <osg/AutoTransform>
#include <Inner/Common/ImplSceneGroup.hpp>

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
    void ScalChanged()SET_TRUE_NODE_UPDATE(m_bScaleChanged)
    void AutoScalChanged()SET_TRUE_NODE_UPDATE(m_bAutoScalChanged)
    void MinScalChanged()SET_TRUE_NODE_UPDATE(m_bMinScalChanged)
    void MaxScalChanged()SET_TRUE_NODE_UPDATE(m_bMaxScalChanged)

    void UpdateNode()
    {
        if(m_bScaleChanged)
        {
            if(!T::m_bAutoScal)
            {
                m_pAutoScaleTransform->setScale(T::m_dScalBit);
            }
            m_bScaleChanged = false;
        }

        if(m_bAutoScalChanged)
        {
            if(!T::m_bAutoScal)
            {
                m_pAutoScaleTransform->setScale(T::m_dScalBit);
            }
            m_pAutoScaleTransform->setAutoScaleToScreen(T::m_bAutoScal);

            m_bAutoScalChanged=false;
        }

        if(m_bMinScalChanged)
        {
            m_pAutoScaleTransform->setMinimumScale(T::m_dMinScal);
            m_bMinScalChanged=false;
        }

        if(m_bMaxScalChanged)
        {
            m_pAutoScaleTransform->setMaximumScale(T::m_dMaxScal);
            m_bMaxScalChanged=false;
        }

        ImplSceneGroup<T>::UpdateNode();
    }

protected:
    osg::observer_ptr<osg::AutoTransform> m_pAutoScaleTransform;
    bool m_bScaleChanged=false;
    bool m_bAutoScalChanged=false;
    bool m_bMinScalChanged=false;
    bool m_bMaxScalChanged=false;
};

#endif // IMPL_SCENE_MODEL_H
