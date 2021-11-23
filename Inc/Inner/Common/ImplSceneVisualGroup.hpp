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

//        osg::Image* pImage = new osg::Image;
//        pImage->allocateImage(1024,1024,1,GL_RGBA,GL_UNSIGNED_BYTE);
//        m_pCamera->attach(osg::Camera::COLOR_BUFFER,pImage);
//        m_pCamera->setPostDrawCallback(new MyCameraPostDrawCallback(pImage));

        m_pTextureProjMatrix = new osg::Uniform(osg::Uniform::FLOAT_MAT4,"textureProjMatrix");
        m_pTextureViewMatrix = new osg::Uniform(osg::Uniform::FLOAT_MAT4,"textureViewMatrix");
        m_pHiddenColor = new osg::Uniform("hiddenColor", osg::Vec4(1.0, 0.0, 0.0, 1.0));
        m_pVisibleColor = new osg::Uniform("visibleColor", osg::Vec4(0.0, 1.0, 0.0, 1.0));
        m_pDepthTexture = new osg::Uniform("depthTexture",1);
        m_pDepthTexture = new osg::Uniform("depthTexture",1);
        m_pFar = new osg::Uniform(osg::Uniform::FLOAT,"fFar");
        m_pNear = new osg::Uniform(osg::Uniform::FLOAT,"fNear");
        m_pCamera->getOrCreateStateSet()->setDefine("VISUAL_CAMERA");

        ImplSceneGroup<T>::SetGroupNode(m_pCamera.get());
    }

    /// 更新
    void FrameCall()
    {
        if(m_bParameterChanged)
        {
            /// 设置投影矩阵
            m_pCamera->setProjectionMatrixAsPerspective(T::m_dFovy,T::m_dAspect,
                                                        T::m_stViewPoint.fDistance*1e-4,T::m_stViewPoint.fDistance);
            m_pFar->set(T::m_stViewPoint.fDistance);
            m_pNear->set(T::m_stViewPoint.fDistance*1e-4);

            osg::Vec3d eye,center,up;
            if(T::m_bIsGeoPos)
            {
                Math::CMatrix local2Global = GisMath::LOCAL2GLOBAL(T::m_stViewPoint.stPos.dX*DD2R,T::m_stViewPoint.stPos.dY*DD2R);
                /// 计算射线方线
                Math::CVector vLocal = Math::CVecMat::VecPolar((90-T::m_stViewPoint.fAzimuth)*DD2R,T::m_stViewPoint.fElev*DD2R
                                                               ,T::m_stViewPoint.fDistance);
                Math::CVector vGlobal=local2Global*vLocal;

                /// 将经纬度转成
                double dX,dY,dZ;
                GisMath::LBH2XYZ(T::m_stViewPoint.stPos.dX*DD2R,T::m_stViewPoint.stPos.dY*DD2R,T::m_stViewPoint.stPos.dZ,
                                 dX,dY,dZ);

                vGlobal(0) += dX;
                vGlobal(1) += dY;
                vGlobal(2) += dZ;

                eye.set(dX,dY,dZ);
                center.set(vGlobal.GetX(),vGlobal.GetY(),vGlobal.GetZ());

                vLocal.Set(0,0,1);
                vGlobal = local2Global*vLocal;
                up.set(vGlobal.GetX(),vGlobal.GetY(),vGlobal.GetZ());
            }
            else
            {
                eye.set(T::m_stViewPoint.stPos.dX,T::m_stViewPoint.stPos.dY,T::m_stViewPoint.stPos.dZ);
                Math::CVector vLocal = Math::CVecMat::VecPolar((90-T::m_stViewPoint.fAzimuth)*DD2R,T::m_stViewPoint.fElev*DD2R
                                                               ,T::m_stViewPoint.fDistance);
                center.set(T::m_stViewPoint.stPos.dX+vLocal.GetX(),
                           T::m_stViewPoint.stPos.dY+vLocal.GetY(),
                           T::m_stViewPoint.stPos.dZ+vLocal.GetZ());
                up.set(0.,0.,1.);
            }
            m_pCamera->setViewMatrixAsLookAt(eye,center,up);

            m_pTextureViewMatrix->set(m_pCamera->getViewMatrix());
            m_pTextureProjMatrix->set(m_pCamera->getProjectionMatrix());
            m_bParameterChanged=false;
        }

        if(m_bChildInsert)
        {
            for(auto one : m_listInsertChild)
            {
                for(auto oneSceneNode :m_setChildNode)
                {
                    if(one == oneSceneNode->AsOsgSceneNode()->GetRealNode())
                    {
                        oneSceneNode->StateSet()|=VISUAL_STATE;
                        one->setStateSet(T::m_pSceneGraph->ResouceLoader()->GetOrCreateStateSet(oneSceneNode->StateSet()));
                    }
                }

                one->getOrCreateStateSet()->setTextureAttribute(1, m_pTexture2D.get());
                one->getOrCreateStateSet()->addUniform(m_pTextureProjMatrix);
                one->getOrCreateStateSet()->addUniform(m_pTextureViewMatrix);
                one->getOrCreateStateSet()->addUniform(m_pDepthTexture);
                one->getOrCreateStateSet()->addUniform(m_pVisibleColor);
                one->getOrCreateStateSet()->addUniform(m_pHiddenColor);
                one->getOrCreateStateSet()->addUniform(m_pFar);
                one->getOrCreateStateSet()->addUniform(m_pNear);
            }
            m_listInsertChild.clear();
            m_bChildInsert=false;
        }

        if(m_bChildRemove)
        {
            m_listInsertChild.clear();
            m_bChildRemove=false;
        }
        ImplSceneGroup<T>::FrameCall();
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
    osg::ref_ptr<osg::Uniform>     m_pTextureViewMatrix;
    osg::ref_ptr<osg::Uniform>     m_pTextureProjMatrix;
    osg::ref_ptr<osg::Uniform>     m_pDepthTexture;
    osg::ref_ptr<osg::Uniform>     m_pVisibleColor;
    osg::ref_ptr<osg::Uniform>     m_pHiddenColor;
    osg::ref_ptr<osg::Uniform>     m_pFar;
    osg::ref_ptr<osg::Uniform>     m_pNear;

    std::list<osg::ref_ptr<osg::Node>> m_listInsertChild;
    bool m_bParameterChanged{false};
    bool m_bChildInsert{false};
    bool m_bChildRemove{false};
};

#endif // IMPL_SCENE_GROUP_H
