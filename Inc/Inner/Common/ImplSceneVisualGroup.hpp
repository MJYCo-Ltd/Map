#ifndef IMPL_SCENE_VISUAL_GROUP_H
#define IMPL_SCENE_VISUAL_GROUP_H

#include <set>

#include <GisMath/GisMath.h>
#include <Inner/Common/ImplSceneGroup.hpp>
#include <Inner/ILoadResource.h>
#include <osgDB/WriteFile>

struct MyCameraPostDrawCallback : public osg::Camera::DrawCallback
{
    MyCameraPostDrawCallback(osg::Image* image):
        _image(image)
    {
    }

    virtual void operator () (const osg::Camera& camera) const
    {
        osgDB::writeImageFile(*_image,"E:/skyup.bmp");
    }
    osg::Image* _image;
};

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
        T::m_bOpenLight=false;
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
        m_pCamera->setComputeNearFarMode(osg::Camera::DO_NOT_COMPUTE_NEAR_FAR);
        m_pCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
        m_pCamera->setRenderOrder(osg::Camera::PRE_RENDER);
        m_pCamera->setViewport(0, 0, 1024, 1024);
        m_pCamera->attach(osg::Camera::DEPTH_BUFFER, m_pTexture2D.get());

        m_pRenderTextureMatrix = new osg::Uniform(osg::Uniform::FLOAT_MAT4,"renderTextureMatrix");
        m_pTextureProjMatrix = new osg::Uniform(osg::Uniform::FLOAT_MAT4,"textureProjMatrix");
        m_pTextureViewMatrix = new osg::Uniform(osg::Uniform::FLOAT_MAT4,"textureViewMatrix");
        m_pHiddenColor = new osg::Uniform("hiddenColor", osg::Vec4(1.0, 0.0, 0.0, 1.0));
        m_pVisibleColor = new osg::Uniform("visibleColor", osg::Vec4(0.0, 1.0, 0.0, 1.0));
        m_pDepthTexture = new osg::Uniform("depthTexture",1);
        m_pDepth = new osg::Uniform(osg::Uniform::FLOAT,"fDistance");
        m_pCamera->getOrCreateStateSet()->setDefine("VISUAL_CAMERA");

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
            m_pDepth->set(T::m_stViewPoint.fDistance);

            Math::CMatrix local2Global = GisMath::LOCAL2GLOBAL(T::m_stViewPoint.stPos.fX*DD2R,T::m_stViewPoint.stPos.fY*DD2R);
            /// 计算射线方线
            Math::CVector vLocal = Math::CVecMat::VecPolar((90-T::m_stViewPoint.fAzimuth)*DD2R,T::m_stViewPoint.fElev*DD2R
                                                           ,T::m_stViewPoint.fDistance);
            Math::CVector vGlobal=local2Global*vLocal;

            /// 将经纬度转成
            double dX,dY,dZ;
            GisMath::LBH2XYZ(T::m_stViewPoint.stPos.fX*DD2R,T::m_stViewPoint.stPos.fY*DD2R,T::m_stViewPoint.stPos.fZ,
                             dX,dY,dZ);

            vGlobal(0) += dX;
            vGlobal(1) += dY;
            vGlobal(2) += dZ;

            osg::Vec3d eye(dX,dY,dZ);
            osg::Vec3d center(vGlobal.GetX(),vGlobal.GetY(),vGlobal.GetZ());

            vLocal.Set(0,0,1);
            vGlobal = local2Global*vLocal;
            osg::Vec3d up(vGlobal.GetX(),vGlobal.GetY(),vGlobal.GetZ());
            m_pCamera->setViewMatrixAsLookAt(eye,center,up);

            osg::Matrixf renderTextureMatrix = m_pCamera->getViewMatrix() * m_pCamera->getProjectionMatrix();
            m_pRenderTextureMatrix->set(renderTextureMatrix);
            m_pTextureViewMatrix->set(m_pCamera->getViewMatrix());
            m_pTextureProjMatrix->set(m_pCamera->getProjectionMatrix());
            m_bParameterChanged=false;
        }

        if(m_bChildInsert)
        {

            for(auto one : m_listInsertChild)
            {
                one->getOrCreateStateSet()->setTextureAttributeAndModes(1, m_pTexture2D.get(), osg::StateAttribute::ON);
                one->getOrCreateStateSet()->addUniform(m_pRenderTextureMatrix);
                one->getOrCreateStateSet()->addUniform(m_pTextureProjMatrix);
                one->getOrCreateStateSet()->addUniform(m_pTextureViewMatrix);
                one->getOrCreateStateSet()->addUniform(m_pDepthTexture);
                one->getOrCreateStateSet()->addUniform(m_pVisibleColor);
                one->getOrCreateStateSet()->addUniform(m_pHiddenColor);
                one->getOrCreateStateSet()->addUniform(m_pDepth);
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
    osg::ref_ptr<osg::Image>       m_pImage;
    osg::ref_ptr<osg::Texture2D>   m_pTexture2D;
    osg::ref_ptr<osg::Uniform>     m_pRenderTextureMatrix;
    osg::ref_ptr<osg::Uniform>     m_pTextureViewMatrix;
    osg::ref_ptr<osg::Uniform>     m_pTextureProjMatrix;
    osg::ref_ptr<osg::Uniform>     m_pDepthTexture;
    osg::ref_ptr<osg::Uniform>     m_pVisibleColor;
    osg::ref_ptr<osg::Uniform>     m_pHiddenColor;
    osg::ref_ptr<osg::Uniform>     m_pDepth;

    std::list<osg::ref_ptr<osg::Node>> m_listInsertChild;
    bool m_bParameterChanged=false;
    bool m_bChildInsert=false;
    bool m_bChildRemove=false;
};

#endif // IMPL_SCENE_GROUP_H
