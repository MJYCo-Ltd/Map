#ifndef IMPL_SCENE_VISUAL_GROUP_H
#define IMPL_SCENE_VISUAL_GROUP_H

#include <set>
#include <Inner/Common/ImplSceneGroup.hpp>

/**
 *  实现ISceneVisualGroup的接口
 */
template <typename T>
class ImplSceneVisualGroup:public ImplSceneGroup<T>
{
public:
    CONSTRUCTOR(ImplSceneVisualGroup,ImplSceneGroup<T>)

protected:
    void InitNode()
    {
        ImplSceneNode<T>::InitNode();

        osg::ref_ptr<osg::Texture2D> texture2D = new osg::Texture2D;
        texture2D->setTextureSize(1024, 1024);
        texture2D->setInternalFormat(GL_DEPTH_COMPONENT);
        texture2D->setSourceFormat(GL_DEPTH_COMPONENT);
        texture2D->setSourceType(GL_FLOAT);
        texture2D->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
        texture2D->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
        texture2D->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
        texture2D->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);

        m_pCamera = new osg::Camera();
        m_pCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
        m_pCamera->setClearColor(osg::Vec4());
        m_pCamera->setComputeNearFarMode(osg::Camera::DO_NOT_COMPUTE_NEAR_FAR);
        m_pCamera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_pCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
        m_pCamera->setRenderOrder(osg::Camera::PRE_RENDER);
        m_pCamera->setViewport(0, 0, texture2D->getTextureWidth(), texture2D->getTextureHeight());
        m_pCamera->attach(osg::Camera::DEPTH_BUFFER, texture2D.get());

        ImplSceneGroup<T>::SetGroupNode(m_pCamera.get());
    }

    /// 更新
    void UpdateNode()
    {
        m_pCamera->setProjectionMatrix(osg::Matrixd::perspective(T::m_dFovy,T::m_dAspect,0.,T::m_dDistance));
        ImplSceneGroup<T>::UpdateNode();
    }

    /// 增加了一个节点
    void InsertChildNode(osg::Node* pNode)
    {
    }

    /// 删除了一个节点
    void RemoveChildNode(osg::Node*)
    {
    }

    /// 参数更新
    void ParameterChanged()SET_TRUE_NODE_UPDATE(m_bParameterChanged)
protected:
    osg::observer_ptr<osg::Camera> m_pCamera;
    bool m_bParameterChanged=false;
};

#endif // IMPL_SCENE_GROUP_H
