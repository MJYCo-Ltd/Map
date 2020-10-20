#ifndef IMPL_SCENE_SENSOR_H
#define IMPL_SCENE_SENSOR_H
#include <osg/Depth>
#include <osg/MatrixTransform>
#include <Inner/ImplSceneShape.hpp>

/**
 *  实现ISceneNode所有的接口
 */
template <typename T>
class ImplSceneSensor:public ImplSceneShape<T>
{
public:
    CONSTRUCTOR(ImplSceneSensor,ImplSceneShape<T>)

protected:

    void DistanceChanged()
    {
        m_bDistanceChanged = true;
        NodeChanged();
    }

    void UpdateNode()
    {
        if(m_bDistanceChanged)
        {
            m_pScalTransform->setMatrix(osg::Matrix::scale(T::m_dDistance,T::m_dDistance,T::m_dDistance));
            m_bDistanceChanged=false;
        }

        ImplSceneShape<T>::UpdateNode();
    }

    void InitNode()
    {
        m_pScalTransform = new osg::MatrixTransform;
        m_pScalTransform->setMatrix(osg::Matrix::scale(T::m_dDistance,T::m_dDistance,T::m_dDistance));
        ImplSceneShape<T>::InitNode();

        auto pState = m_pGeometry->getOrCreateStateSet();
        /// 开启颜色混合 关闭光照
        pState->setMode(GL_BLEND,osg::StateAttribute::ON);
        pState->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
        /// 关闭写深度缓存
        osg::Depth* pDepth = new osg::Depth;
        pDepth->setWriteMask(false);
        pState->setAttribute(pDepth);

        m_pScalTransform->addChild(m_pGeometry.get());
        SetOsgNode(m_pScalTransform.get());
    }
protected:
    osg::observer_ptr<osg::MatrixTransform> m_pScalTransform;
    bool       m_bDistanceChanged=false;
};

#endif // IMPL_SCENE_SENSOR_H
