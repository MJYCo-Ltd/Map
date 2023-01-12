#ifndef IMPL_SCENE_VISUAL_GROUPNew_H
#define IMPL_SCENE_VISUAL_GROUPNew_H

#include <set>

#include <GisMath/GisMath.h>
#include <Inner/Common/ImplSceneGroup.hpp>
#include <Inner/ILoadResource.h>
#include <Plot/Map/IMapLocation.h>
#include <osgDB/WriteFile>

/**
 *  实现ISceneVisualGroupNew的接口
 */
template <typename T>
class ImplSceneVisualGroupNew:public ImplSceneGroup<T>
{
public:
    CONSTRUCTOR(ImplSceneVisualGroupNew,ImplSceneGroup<T>)

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
        if(m_bParameterChanged||m_bChildInsert)
        {
            /// 设置投影矩阵
            m_pCamera->setProjectionMatrixAsPerspective(T::m_dFovy,T::m_dAspect,
                                                        T::m_stViewPoint.fDistance*1e-4,T::m_stViewPoint.fDistance);
            m_pFar->set(T::m_stViewPoint.fDistance);
            m_pNear->set(float(T::m_stViewPoint.fDistance*1e-4));


            IMapLocation* temp_Location=nullptr;
            for(auto one : m_listInsertChild)
            {
                for(auto oneSceneNode :this->m_setChildNode)
                {
                    if(one == oneSceneNode->AsOsgSceneNode()->GetRealNode())
                    {
                        oneSceneNode->StateSet()|=VISUAL_STATE;
                        one->setStateSet(T::m_pSceneGraph->ResouceLoader()->GetOrCreateStateSet(oneSceneNode->StateSet()));
                        temp_Location=dynamic_cast<IMapLocation*>(oneSceneNode);
                    }
                }

                if(temp_Location)
                {
                    //摄像机位置
                    ScenePos temp_CameraPos=T::m_stViewPoint.stPos;
                    //模型的位置
                    ScenePos temp_LocationPos=temp_Location->GeoPos();

                    //模型的经纬度或者是XYZ(与T::m_bIsGeoPos有关)
                    double temp_LocalX,temp_LocalY,temp_LocalZ;
                    temp_LocalX=temp_LocalY=temp_LocalZ=0;
                    temp_LocalX=temp_LocationPos.dX;
                    temp_LocalY=temp_LocationPos.dY;
                    temp_LocalZ=temp_LocationPos.dZ;

                    //calculate camera location pos
                    osg::Vec3d eye,center,up;
                    // 位置是否是地理信息 xyz
                    if(T::m_bIsGeoPos)
                    {


                        //摄像机：局部东北天坐标 转换成 全局坐标 的转换矩阵
                        Math::CMatrix local2Global = GisMath::LOCAL2GLOBAL(T::m_stViewPoint.stPos.dX*DD2R,T::m_stViewPoint.stPos.dY*DD2R);
                        //摄像机： 计算射线方线
                        Math::CVector vLocal = Math::CVecMat::VecPolar((90-T::m_stViewPoint.fAzimuth)*DD2R,T::m_stViewPoint.fElev*DD2R
                                                                       ,T::m_stViewPoint.fDistance);
                        //得出摄像机的
                        Math::CVector vGlobal=local2Global*vLocal;

                        // 将摄像机的经纬度转成 xyz
                        double dX,dY,dZ;
                        GisMath::LBH2XYZ(T::m_stViewPoint.stPos.dX*DD2R,T::m_stViewPoint.stPos.dY*DD2R,T::m_stViewPoint.stPos.dZ,
                                         dX,dY,dZ);
                        //计算摄像机中心点xyz
                        vGlobal(0) += dX;
                        vGlobal(1) += dY;
                        vGlobal(2) += dZ;



                        //摄像机的位置-模型位置=相对的位置（xyz）
                        ScenePos tempPos;
                        tempPos.dX=dX-temp_LocalX;
                        tempPos.dY=dY-temp_LocalY;
                        tempPos.dZ=dZ-temp_LocalZ;
                        eye.set(tempPos.dX,tempPos.dY,tempPos.dZ);

                        //计算摄像机中心点相对位置（xyz)
                        tempPos.dX=vGlobal.GetX()-temp_LocalX;
                        tempPos.dY=vGlobal.GetY()-temp_LocalY;
                        tempPos.dZ=vGlobal.GetZ()-temp_LocalZ;
                        center.set(tempPos.dX,tempPos.dY,tempPos.dZ);

                        //计算上方点位置
                        vLocal.Set(0,0,1);
                        vGlobal = local2Global*vLocal;
                        tempPos.dX=vGlobal.GetX();
                        tempPos.dY=vGlobal.GetY();
                        tempPos.dZ=vGlobal.GetZ();
                        up.set(tempPos.dX,tempPos.dY,tempPos.dZ);
                    }
                    else
                    {
                        eye.set(T::m_stViewPoint.stPos.dX-temp_LocalX,
                                T::m_stViewPoint.stPos.dY-temp_LocalY,
                                T::m_stViewPoint.stPos.dZ-temp_LocalZ);
                        Math::CVector vLocal = Math::CVecMat::VecPolar(
                                    (90-T::m_stViewPoint.fAzimuth)*DD2R,
                                    T::m_stViewPoint.fElev*DD2R,
                                    T::m_stViewPoint.fDistance);
                        center.set(T::m_stViewPoint.stPos.dX+vLocal.GetX()-temp_LocalX,
                                   T::m_stViewPoint.stPos.dY+vLocal.GetY()-temp_LocalY,
                                   T::m_stViewPoint.stPos.dZ+vLocal.GetZ()-temp_LocalZ);
                        up.set(0.,0.,1.);
                    }
                    m_pCamera->setViewMatrixAsLookAt(eye,center,up);

                    m_pTextureViewMatrix->set(m_pCamera->getViewMatrix());
                    m_pTextureProjMatrix->set(m_pCamera->getProjectionMatrix());
                    //m_bParameterChanged=false;


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

        ImplSceneGroup<T>::FrameCall();
    }

    /// 增加了一个节点
    void InsertChildNode(osg::Node* pNode)
    {
        SET_TRUE_NODE_UPDATE(m_bChildInsert)
        m_listInsertChild.push_back(pNode);

    }

    /// 删除了一个节点
    void RemoveChildNode(osg::Node* pNode)
    {
        for(auto oneSceneNode :this->m_setChildNode)
        {
            if(pNode == oneSceneNode->AsOsgSceneNode()->GetRealNode())
            {
                oneSceneNode->RemoveStateSet(VISUAL_STATE);
                break;
            }
        }
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
};

#endif // IMPL_SCENE_GROUP_H
