#ifndef IMPL_SCENE_VISUAL_GROUP_H
#define IMPL_SCENE_VISUAL_GROUP_H

#include <set>

#include <GisMath/GisMath.h>
#include <Inner/Common/ImplSceneGroup.hpp>
#include <Inner/ILoadResource.h>

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

        m_pTexture2D = new osg::Texture2D;
        m_pTexture2D->setTextureSize(1024, 1024);
        m_pTexture2D->setInternalFormat(GL_DEPTH_COMPONENT);
        m_pTexture2D->setSourceFormat(GL_DEPTH_COMPONENT);
        m_pTexture2D->setSourceType(GL_FLOAT);
        m_pTexture2D->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
        m_pTexture2D->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
        m_pTexture2D->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
        m_pTexture2D->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);

        m_pCamera = new osg::Camera();
        m_pCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
        m_pCamera->setClearColor(osg::Vec4());
        m_pCamera->setComputeNearFarMode(osg::Camera::DO_NOT_COMPUTE_NEAR_FAR);
        m_pCamera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_pCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
        m_pCamera->setRenderOrder(osg::Camera::PRE_RENDER);
        m_pCamera->setViewport(0, 0, m_pTexture2D->getTextureWidth(), m_pTexture2D->getTextureHeight());
        m_pCamera->attach(osg::Camera::DEPTH_BUFFER, m_pTexture2D.get());

        ImplSceneGroup<T>::SetGroupNode(m_pCamera.get());
    }

    /// 更新
    void UpdateNode()
    {
        if(m_bParameterChanged)
        {
            /// 设置投影矩阵
            m_pCamera->setProjectionMatrixAsPerspective(T::m_dFovy,T::m_dAspect,
                                                        T::m_stViewPoint.fDistance*1e-4,T::m_stViewPoint.fDistance);
            m_bParameterChanged=false;

            Math::CMatrix local2Global = GisMath::LOCAL2GLOBAL(T::m_stViewPoint.stPos.fX*DD2R,T::m_stViewPoint.stPos.fY*DD2R);
            /// 计算射线方线
            Math::CVector vLocal = Math::CVecMat::VecPolar((DPI/2.-T::m_stViewPoint.fAzimuth)*DD2R,T::m_stViewPoint.fAzimuth*DD2R);
            Math::CVector vGlobal=local2Global*vLocal;

            /// 将经纬度转成
            double dX,dY,dZ;
            GisMath::LBH2XYZ(T::m_stViewPoint.stPos.fX*DD2R,T::m_stViewPoint.stPos.fY*DD2R,T::m_stViewPoint.stPos.fZ,
                             dX,dY,dZ);

            vGlobal(0) += dX;
            vGlobal(1) += dY;
            vGlobal(2) += dZ;

            osg::Vec3 eye(dX,dY,dZ);
            osg::Vec3 center(vGlobal.GetX(),vGlobal.GetY(),vGlobal.GetZ());

            vLocal.Set(0,0,1);
            vGlobal = local2Global*vLocal;
            osg::Vec3 up(vGlobal.GetX(),vGlobal.GetY(),vGlobal.GetZ());
            m_pCamera->setViewMatrixAsLookAt(eye,center,up);
        }

        if(m_bChildInsert)
        {
            osg::Matrixf renderTextureMatrix = m_pCamera->getViewMatrix() * m_pCamera->getProjectionMatrix();
            for(auto one : m_listInsertChild)
            {
                one->getOrCreateStateSet()->setTextureAttributeAndModes(1, m_pTexture2D.get(), osg::StateAttribute::ON);
                one->getOrCreateStateSet()->addUniform(new osg::Uniform("renderTextureMatrix", renderTextureMatrix));
                one->getOrCreateStateSet()->addUniform(new osg::Uniform("depthTexture", 1));
                one->getOrCreateStateSet()->addUniform(new osg::Uniform("visibleColor", osg::Vec4(0.0, 1.0, 0.0, 1.0)));
                one->getOrCreateStateSet()->addUniform(new osg::Uniform("hiddenColor", osg::Vec4(1.0, 0.0, 0.0, 1.0)));
                auto pVirutlProgram = osgEarth::VirtualProgram::getOrCreate(one->getOrCreateStateSet());
                T::m_pSceneGraph->ResouceLoader()->LoadVirtualProgram(pVirutlProgram,"GLSL/Visual.glsl");
            }
            m_listInsertChild.clear();
            m_bChildInsert=false;
        }

        if(m_bChildRemove)
        {
            for(auto one : m_listInsertChild)
            {
                auto pVirutlProgram = osgEarth::VirtualProgram::getOrCreate(one->getOrCreateStateSet());
                T::m_pSceneGraph->ResouceLoader()->RemoveVirtualProgram(pVirutlProgram,"GLSL/Visual.glsl");
            }
            m_listInsertChild.clear();
            m_bChildRemove=false;
        }
        ImplSceneGroup<T>::UpdateNode();
    }

    /// 增加了一个节点
    void InsertChildNode(osg::Node* pNode)
    {
        m_bChildInsert=true;
        m_listInsertChild.push_back(pNode);
    }

    /// 删除了一个节点
    void RemoveChildNode(osg::Node* pNode)
    {
        m_bChildRemove=true;
        m_listInsertChild.push_back(pNode);
    }

    /// 参数更新
    void ParameterChanged()SET_TRUE_NODE_UPDATE(m_bParameterChanged)
protected:
    osg::observer_ptr<osg::Camera> m_pCamera;
    osg::ref_ptr<osg::Texture2D>   m_pTexture2D;
    std::list<osg::ref_ptr<osg::Node>> m_listInsertChild;
    bool m_bParameterChanged=false;
    bool m_bChildInsert=false;
    bool m_bChildRemove=false;
};

#endif // IMPL_SCENE_GROUP_H
