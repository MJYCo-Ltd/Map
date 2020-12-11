#ifndef IMPL_SCENE_SENSOR_H
#define IMPL_SCENE_SENSOR_H
#include <osg/PolygonMode>
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

    /**
    * @brief 显示类型修改
    */
    void ShowTypeChanged()
    {
        m_bShowTypeChanged = true;
        ImplSceneShape<T>::NodeChanged();
    }

    /**
     * @brief 显示距离修改
     */
    void DistanceChanged()
    {
        m_bDistanceChanged = true;
        ImplSceneShape<T>::NodeChanged();
    }

    void UpdateNode()
    {
        if(m_bDistanceChanged)
        {
            m_pScalTransform->setMatrix(osg::Matrix::scale(T::m_dDistance,T::m_dDistance,T::m_dDistance));
            m_bDistanceChanged=false;
        }

        if(m_bShowTypeChanged)
        {
            if(T::m_bShowLine)
            {
                m_pLineGroup->setNodeMask(0xffffffffu);
            }
            else
            {
                m_pLineGroup->setNodeMask(0);
            }

            if(T::m_bShowFace)
            {
                m_pFaceGroup->setNodeMask(0xffffffffu);
            }
            else
            {
                m_pFaceGroup->setNodeMask(0);
            }
            m_bShowTypeChanged=false;
        }

        ImplSceneShape<T>::UpdateNode();
    }

    /// 重写初始化状态
    void InitNode()
    {
        ImplSceneShape<T>::InitNode();

        m_pLineGroup = new osg::Group;
        m_pFaceGroup = new osg::Group;
        m_pScalTransform = new osg::MatrixTransform;

        m_pScalTransform->setMatrix(osg::Matrix::scale(T::m_dDistance,T::m_dDistance,T::m_dDistance));
        m_pScalTransform->addChild(m_pLineGroup.get());
        m_pScalTransform->addChild(m_pFaceGroup.get());

        m_pLineGroup->addChild(ImplSceneShape<T>::m_pGeometry.get());
        m_pFaceGroup->addChild(ImplSceneShape<T>::m_pGeometry.get());

        /// 线模型只绘制线 面模型只绘制面
        m_pLineGroup->getOrCreateStateSet()->setAttributeAndModes(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE));
        m_pFaceGroup->getOrCreateStateSet()->setAttributeAndModes(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::FILL));

        ImplSceneShape<T>::SetOsgNode(m_pScalTransform.get());
    }
protected:
    osg::observer_ptr<osg::Group>    m_pFaceGroup;
    osg::observer_ptr<osg::Group>    m_pLineGroup;
    osg::observer_ptr<osg::MatrixTransform> m_pScalTransform;
    bool       m_bDistanceChanged=false;
    bool       m_bShowTypeChanged=false;
};

#endif // IMPL_SCENE_SENSOR_H
