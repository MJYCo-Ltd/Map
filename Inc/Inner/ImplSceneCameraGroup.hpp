#ifndef IMPL_SCENE_CAMERA_GROUP_H
#define IMPL_SCENE_CAMERA_GROUP_H

#include <set>
#include <Inner/ImplSceneGroup.hpp>

/**
 *  实现ISceneCameraGroup的接口
 */
template <typename T>
class ImplSceneCameraGroup:public ImplSceneGroup<T>
{
public:
    CONSTRUCTOR(ImplSceneCameraGroup,ImplSceneGroup<T>)

protected:
    void InitNode()
    {
        ImplSceneNode<T>::InitNode();
        m_pCamera = new osg::Camera;
        m_pCamera->setRenderOrder( osg::Camera::NESTED_RENDER );
        m_pCamera->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES);
        ImplSceneGroup<T>::SetGroupNode(m_pCamera.get());
    }

    void UpdateNode()
    {
        if(m_bIndexChanged)
        {
            m_pCamera->getOrCreateStateSet()->setRenderBinDetails(T::m_nIndex, "RenderBin" );
            m_bIndexChanged=false;
        }

        ImplSceneGroup<T>::UpdateNode();
    }

    void RenderIndexChanged()SET_TRUE_NODE_UPDATE(m_bIndexChanged)
protected:
    osg::observer_ptr<osg::Camera> m_pCamera;
    bool m_bIndexChanged=false;
};

#endif // IMPL_SCENE_GROUP_H
