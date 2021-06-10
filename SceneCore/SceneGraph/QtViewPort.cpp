#include <osg/Stencil>
#include <osg/PolygonStipple>
#include <osgEarth/GeoData>
#include <osgViewer/ViewerEventHandlers>

#include <Inner/IRender.h>
#include <Inner/IOsgSceneNode.h>

#include "MyEarthManipulator.h"
#include "QtViewPort.h"
#include "QtViewHud.h"
#include "DealViewPortChange.h"
#include "ViewPortEventCallback.h"


/// 截屏
struct CaptureImageCallback:public osg::Camera::DrawCallback
{
    CaptureImageCallback(QtViewPort* pViewPort):m_pViewPort(pViewPort)
    {
        m_pImage = new osg::Image;
    }

    virtual void operator () (osg::RenderInfo& renderInfo) const
    {
        osg::GraphicsContext* gc = renderInfo.getState()->getGraphicsContext();
        int width =gc->getTraits()->width, height=gc->getTraits()->height;

        ///读取像素信息抓图
        m_pImage->readPixels(0 , 0 , width , height , GL_RGB , GL_UNSIGNED_BYTE);

        /// 发送消息
        QMetaObject::invokeMethod(m_pViewPort,"CaptureImage",Q_ARG(int,width),
                                  Q_ARG(int,height),
                                  Q_ARG(QByteArray,QByteArray::fromRawData(reinterpret_cast<const char *>(m_pImage->data()),
                                                                           m_pImage->getTotalSizeInBytes())));
    }
private:
    osg::ref_ptr<osg::Image> m_pImage;
    QtViewPort* m_pViewPort;
};

#ifdef NEED_VR
#include <openvr/openvr.h>
#include <QDebug>

struct VRSlaveCallback : public osg::View::Slave::UpdateSlaveCallback
{
    VRSlaveCallback(double eyeScale):_eyeScale(eyeScale) {}

    virtual void updateSlave(osg::View& view, osg::View::Slave& slave)
    {
        osg::Camera* camera = slave._camera.get();
        osgViewer::View* viewer_view = dynamic_cast<osgViewer::View*>(&view);
        if (camera && viewer_view)
        {
//            camera->inheritCullSettings(*(view.getCamera()), camera->getInheritanceMask());

            if (_eyeScale<0.0)
            {
                camera->setCullMask(camera->getCullMaskLeft());
            }
            else
            {
                camera->setCullMask(camera->getCullMaskRight());
            }

            double fovy, aspectRatio,zNear,zFar;
            view.getCamera()->getProjectionMatrixAsPerspective(fovy, aspectRatio,zNear,zFar);
            camera->setProjectionMatrixAsPerspective(fovy,1,zNear,zFar);


            double sd = osg::DisplaySettings::instance()->getScreenDistance();
            double fusionDistance = sd;
            switch(viewer_view->getFusionDistanceMode())
            {
            case(osgUtil::SceneView::USE_FUSION_DISTANCE_VALUE):
                fusionDistance = viewer_view->getFusionDistanceValue();
                break;
            case(osgUtil::SceneView::PROPORTIONAL_TO_SCREEN_DISTANCE):
                fusionDistance *= viewer_view->getFusionDistanceValue();
                break;
            }
            double eyeScale = osg::absolute(_eyeScale) * (fusionDistance/sd);

            if (_eyeScale<0.0)
            {
                camera->setViewMatrix(osg::DisplaySettings::instance()->computeLeftEyeViewImplementation(view.getCamera()->getViewMatrix(), eyeScale));
            }
            else
            {
                camera->setViewMatrix(osg::DisplaySettings::instance()->computeRightEyeViewImplementation(view.getCamera()->getViewMatrix(), eyeScale));
            }
        }
    }

    double _eyeScale;
};

struct VRCallback:public osg::Camera::DrawCallback
{
    VRCallback(osg::Texture* pTexture,int nID):m_pTexture(pTexture),m_nID(nID){}
    virtual void operator () (osg::RenderInfo& renderInfo) const
    {
        osg::State& state = *renderInfo.getState();
        const unsigned int contextID = state.getContextID();

        // get the texture object for the current contextID.
        osg::Texture::TextureObject* textureObject = m_pTexture->getTextureObject(contextID);
        vr::Texture_t EyeTexture = {(void*)textureObject->_id, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
        if(0 > m_nID)
        {
            vr::VRCompositor()->Submit(vr::Eye_Left, &EyeTexture);
        }
        else
        {
            vr::VRCompositor()->Submit(vr::Eye_Right, &EyeTexture);
        }
    }
private:
    osg::ref_ptr<osg::Texture> m_pTexture;
    int m_nID{};
};

#endif

/// 视点
QtViewPort::QtViewPort(IRender *pRender,ISceneGraph *pSceneGraph):
    m_pSceneGraph(pSceneGraph),
    m_pRender(pRender)
{
    m_pView = new osgViewer::View;

    m_pView->getCamera()->setViewport(0,0,C_WINDOW_WIDTH,C_WINDOW_HEIGHT);
    m_pView->getCamera()->setClearColor(osg::Vec4(0,0,0,1));

    m_pView->getCamera()->setDrawBuffer(GL_BACK);
    m_pView->getCamera()->setReadBuffer(GL_BACK);
    switch(m_emProjectType)
    {
    case Perspective:
        m_pView->getCamera()->setProjectionMatrixAsPerspective(45,1.0f,0.01,100.);
        break;
    case Ortho:
        m_pView->getCamera()->setProjectionMatrixAsOrtho2D(0,C_WINDOW_WIDTH,0,C_WINDOW_HEIGHT);
        break;
    }

    m_pView->addEventHandler(new osgViewer::StatsHandler);
    m_pView->addEventHandler(new ViewPortEventCallback(this));

    m_pSelfManipulator = new osgGA::TrackballManipulator;
    m_pView->setCameraManipulator(m_pSelfManipulator);

    /// 默认的
    m_stHomePoint.fAzimuth = 0.f;
    m_stHomePoint.fElev = 90.f;
    m_stHomePoint.fDistance = 12000e3;
    m_stHomePoint.stPos.fX = 118.8f;
    m_stHomePoint.stPos.fY = 32.1f;
}

/// 析构函数
QtViewPort::~QtViewPort()
{
    delete m_pHud;
}

/// 地图类型切换
void QtViewPort::ViewPointTypeChanged(ViewPointType emType)
{
    if(emType == m_emType)
    {
        return;
    }

    switch(emType)
    {
    case View_2DMap:
        if(!m_p2DEarthManipulator.valid())
        {
            m_p2DEarthManipulator = new CMyEarthManipulator(MAP_2D);
            m_p2DEarthManipulator->InitHomePoint(m_stHomePoint);
        }
        m_pRender->AddUpdateOperation(new ChangeManipulator(m_pView,m_p2DEarthManipulator));
        break;
    case View_3DMap:
        if(!m_p3DEarthManipulator.valid())
        {
            m_p3DEarthManipulator = new CMyEarthManipulator(MAP_3D);
            m_p3DEarthManipulator->InitHomePoint(m_stHomePoint);
        }
        m_pRender->AddUpdateOperation(new ChangeManipulator(m_pView,m_p3DEarthManipulator));
        break;
    case View_Osg:
        if(!m_pSelfManipulator.valid())
        {
            m_pSelfManipulator = new osgGA::TrackballManipulator;
        }
        m_pRender->AddUpdateOperation(new ChangeManipulator(m_pView,m_pSelfManipulator));
        break;
    case View_Node:
        break;
    }

    m_emType = emType;
}

/// 获取屏显节点
IViewHud *QtViewPort::GetHud()
{
    if(nullptr == m_pHud)
    {
        m_pHud = new QtViewHud(m_pView,m_pSceneGraph);
        m_pHud->Init();
        m_pSceneGraph->GetRoot()->AddSceneNode(m_pHud);
    }
    return(m_pHud);
}

/// 设置视点
void QtViewPort::SetViewPoint(const SceneViewPoint & rViewPoint, unsigned int unTimes)
{
    m_stViewPoint = rViewPoint;
    m_dTimes = unTimes;
    m_bViewPointChanged=true;
}

/// 获取视点位置
const SceneViewPoint &QtViewPort::GetViewPoint()
{
    if(m_bViewPointChanged)
    {
        return(m_stViewPoint);
    }
    else
    {
        switch(m_emType)
        {
        case View_2DMap:
            UpdateViewPoint(m_p2DEarthManipulator);
            break;
        case View_3DMap:
            UpdateViewPoint(m_p3DEarthManipulator);
            break;
        case View_Osg:
            UpdateViewPoint(m_pSelfManipulator);
            break;
        case View_Node:
            UpdateViewPoint(m_pTrackManipulator);
            break;
        }

        return(m_stViewPoint);
    }
}

/// 获取视图类
osgViewer::View *QtViewPort::GetOsgView()
{
    return(m_pView.get());
}

/// 每一帧都会调用一次
void QtViewPort::FrameEvent()
{
    if(m_bStereoChanged)
    {
        RemoveSlave();

        osg::DisplaySettings* ds= m_pView->getDisplaySettings();
        if(nullptr == ds)
        {
            ds =new osg::DisplaySettings(*osg::DisplaySettings::instance());
            m_pView->setDisplaySettings(ds);
        }

        ds->setStereo(false);
        if(m_bOpenStereo)
        {
            ds->setStereo(true);
            ds->setStereoMode(osg::DisplaySettings::StereoMode(m_emStereo));

            int nWidth(C_WINDOW_WIDTH),nHeight(C_WINDOW_HEIGHT);
            auto pViewport = m_pView->getCamera()->getViewport();
            if(pViewport)
            {
                nWidth=pViewport->width();
                nHeight=pViewport->height();
            }

            auto gc = m_pView->getCamera()->getGraphicsContext();
            switch(m_emStereo)
            {
            case QUAD_BUFFER:
            {
                osg::ref_ptr<osg::Camera> left_camera = m_pView->assignStereoCamera(ds, gc,0, 0, nWidth, nHeight, GL_BACK_LEFT, -1.0);
                left_camera->setClearMask(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
                left_camera->setRenderOrder(osg::Camera::NESTED_RENDER, 0);

                m_listStereoCamera.push_back(left_camera);
                osg::ref_ptr<osg::Camera> right_camera = m_pView->assignStereoCamera(ds, gc,0, 0, nWidth, nHeight,GL_BACK_RIGHT, 1.0);
                right_camera->setClearMask(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
                right_camera->setRenderOrder(osg::Camera::NESTED_RENDER, 1);
                m_listStereoCamera.push_back(right_camera);
                break;
            }
            case ANAGLYPHIC:
            {
                osg::ref_ptr<osg::Camera> left_camera = m_pView->assignStereoCamera(ds, gc,0, 0,nWidth, nHeight, GL_BACK, -1.0);
                left_camera->setClearMask(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
                left_camera->getOrCreateStateSet()->setAttribute(new osg::ColorMask(true, false, false, true));
                left_camera->setRenderOrder(osg::Camera::NESTED_RENDER, 0);

                m_listStereoCamera.push_back(left_camera);

                // right Camera right buffer
                osg::ref_ptr<osg::Camera> right_camera = m_pView->assignStereoCamera(ds, gc, 0, 0,nWidth, nHeight, GL_BACK, 1.0);
                right_camera->setClearMask(GL_DEPTH_BUFFER_BIT);
                right_camera->getOrCreateStateSet()->setAttribute(new osg::ColorMask(false, true, true, true));
                right_camera->setRenderOrder(osg::Camera::NESTED_RENDER, 1);
                m_listStereoCamera.push_back(right_camera);
                break;
            }
            case HORIZONTAL_SPLIT:
            {
                bool left_eye_left_viewport = ds->getSplitStereoHorizontalEyeMapping()==osg::DisplaySettings::LEFT_EYE_LEFT_VIEWPORT;
                int left_start = left_eye_left_viewport ? 0 : nWidth/2;
                int right_start = left_eye_left_viewport ? nWidth/2 : 0;

                m_listStereoCamera.push_back(m_pView->assignStereoCamera(ds,gc,left_start,0,nWidth/2,nHeight,GL_BACK,-1.0));
                m_listStereoCamera.push_back(m_pView->assignStereoCamera(ds,gc,right_start,0,nWidth/2,nHeight,GL_BACK,1.0));
                break;
            }
            case VERTICAL_SPLIT:
            {
                bool left_eye_bottom_viewport = ds->getSplitStereoVerticalEyeMapping()==osg::DisplaySettings::LEFT_EYE_BOTTOM_VIEWPORT;
                int left_start = left_eye_bottom_viewport ? 0 : nHeight/2;
                int right_start = left_eye_bottom_viewport ? nHeight/2 : 0;

                m_listStereoCamera.push_back(m_pView->assignStereoCamera(ds,gc,0,left_start,nWidth,nHeight/2,GL_BACK,-1.0));
                m_listStereoCamera.push_back(m_pView->assignStereoCamera(ds,gc,0,right_start,nWidth,nHeight/2,GL_BACK,1.0));
                break;
            }
            }
            m_pView->getCamera()->setGraphicsContext(0);
        }
        m_bStereoChanged=false;
    }
#ifdef NEED_VR

    if(m_bVRStatusChanged)
    {
        if(m_bOpenVR)
        {
            osg::DisplaySettings::instance()->setDisplayType(osg::DisplaySettings::HEAD_MOUNTED_DISPLAY);
            osg::DisplaySettings::instance()->setScreenDistance(1.5f);
            auto gc = m_pView->getCamera()->getGraphicsContext();

            /// 左眼看到的
            {
                osg::ref_ptr<osg::Texture2D> left_texture = new osg::Texture2D;

                left_texture->setTextureSize(m_nVrWidth, m_nVrHeight);
                left_texture->setInternalFormat(GL_RGBA);
                left_texture->setFilter(osg::Texture::MIN_FILTER,osg::Texture::LINEAR);
                left_texture->setFilter(osg::Texture::MAG_FILTER,osg::Texture::LINEAR);
                left_texture->setWrap(osg::Texture::WRAP_S,osg::Texture::CLAMP_TO_EDGE);
                left_texture->setWrap(osg::Texture::WRAP_T,osg::Texture::CLAMP_TO_EDGE);
                osg::ref_ptr<osg::Camera> left_camera = m_pView->assignRenderToTextureCamera(gc,m_nVrWidth, m_nVrHeight,left_texture);
                left_camera->setRenderOrder(osg::Camera::NESTED_RENDER, 0);
                left_camera->setPostDrawCallback(new VRCallback(left_texture.get(),-1));

                osg::View::Slave& left_slave = m_pView->getSlave(m_pView->getNumSlaves()-1);
                left_slave._updateSlaveCallback =  new VRSlaveCallback(-1.);

                m_listVRCamera.push_back(left_camera);
            }

            /// 右眼看到的
            {
                osg::ref_ptr<osg::Texture2D> right_texture = new osg::Texture2D;

                right_texture->setTextureSize(m_nVrWidth, m_nVrHeight);
                right_texture->setInternalFormat(GL_RGBA);
                right_texture->setFilter(osg::Texture::MIN_FILTER,osg::Texture::LINEAR);
                right_texture->setFilter(osg::Texture::MAG_FILTER,osg::Texture::LINEAR);
                right_texture->setWrap(osg::Texture::WRAP_S,osg::Texture::CLAMP_TO_EDGE);
                right_texture->setWrap(osg::Texture::WRAP_T,osg::Texture::CLAMP_TO_EDGE);
                osg::ref_ptr<osg::Camera> right_camera = m_pView->assignRenderToTextureCamera(gc,m_nVrWidth, m_nVrHeight,right_texture);
                right_camera->setRenderOrder(osg::Camera::NESTED_RENDER, 1);
                right_camera->setPostDrawCallback(new VRCallback(right_texture.get(),1));

                osg::View::Slave& right_slave = m_pView->getSlave(m_pView->getNumSlaves()-1);
                right_slave._updateSlaveCallback =  new VRSlaveCallback(1.);

                m_listVRCamera.push_back(right_camera);
            }
        }
        else
        {
            if(m_listVRCamera.size() > 0)
            {
                for(auto one : m_listVRCamera)
                {
                    m_pView->removeSlave(m_pView->findSlaveIndexForCamera(one));
                }
                m_listVRCamera.clear();
            }
        }
        m_bVRStatusChanged=false;
    }

    if(m_bOpenVR)
    {
        vr::VRCompositor()->SetTrackingSpace(vr::TrackingUniverseSeated);

        vr::TrackedDevicePose_t poses[vr::k_unMaxTrackedDeviceCount];
        for (int i = 0; i < vr::k_unMaxTrackedDeviceCount; ++i) poses[i].bPoseIsValid = false;
        vr::VRCompositor()->WaitGetPoses(poses, vr::k_unMaxTrackedDeviceCount, NULL, 0);
    }
#endif
    /// home视点更改
    if(m_bHomePointChanged)
    {
        switch(m_emType)
        {
        case View_2DMap:
            if(m_p2DEarthManipulator.valid())
            {
                m_p2DEarthManipulator->InitHomePoint(m_stHomePoint);
            }
            break;
        case View_3DMap:
            if(m_p3DEarthManipulator)
            {
                m_p3DEarthManipulator->InitHomePoint(m_stHomePoint);
            }
            break;
        default:
            break;
        }

        m_bHomePointChanged=false;
    }

    /// 视点更改
    if(m_bViewPointChanged)
    {
        switch(m_emType)
        {
        case View_3DMap:
            m_p3DEarthManipulator->SetViewPoint(m_stViewPoint,m_dTimes);
            break;
        case View_2DMap:
            m_p2DEarthManipulator->SetViewPoint(m_stViewPoint,m_dTimes);
            break;
        case View_Node:
            m_pTrackManipulator->setDistance(m_stViewPoint.fDistance);
            m_pTrackManipulator->setHeading(osg::DegreesToRadians(m_stViewPoint.fAzimuth));
            m_pTrackManipulator->setElevation(osg::DegreesToRadians(m_stViewPoint.fElev));
            break;
        case View_Osg:
            m_pSelfManipulator->setDistance(m_stViewPoint.fDistance);
            m_pSelfManipulator->setCenter(osg::Vec3(m_stViewPoint.stPos.fX,m_stViewPoint.stPos.fY,
                                                    m_stViewPoint.stPos.fZ));
            m_pSelfManipulator->setHeading(osg::DegreesToRadians(m_stViewPoint.fAzimuth));
            m_pSelfManipulator->setElevation(osg::DegreesToRadians(m_stViewPoint.fElev));
            break;
        default:
            break;
        }
        m_bViewPointChanged=false;
    }

    /// 更新跟踪节点
    if(m_bTraceNodeChanged)
    {
        if(m_pTrackNode->GetBoundSceneGraph() == m_pSceneGraph)
        {
            IOsgSceneNode* pOsgNode = m_pTrackNode->AsOsgSceneNode();

            if(pOsgNode)
            {
                if(!m_pTrackManipulator.valid())
                {
                    m_pTrackManipulator = new osgGA::NodeTrackerManipulator;
                    m_pTrackManipulator->setDistance(m_stViewPoint.fDistance);

                    m_emPreType = m_emType;
                    m_emType = View_Node;

                    m_pTrackManipulator->setTrackNode(pOsgNode->GetOsgNode());
                    m_pRender->AddUpdateOperation(new ChangeManipulator(m_pView,m_pTrackManipulator));
                }
                else
                {
                    m_pTrackManipulator->setRotationMode(osgGA::NodeTrackerManipulator::ELEVATION_AZIM);
                    m_emPreType = m_emType;
                    m_emType = View_Node;
                    m_pTrackManipulator->setTrackNode(pOsgNode->GetOsgNode());
                }
            }
            else
            {
                switch (m_emPreType)
                {
                case View_3DMap:
                    m_pRender->AddUpdateOperation(new ChangeManipulator(m_pView,m_p3DEarthManipulator));
                    break;
                case View_2DMap:
                    m_pRender->AddUpdateOperation(new ChangeManipulator(m_pView,m_p2DEarthManipulator));
                    break;
                default:
                    m_pRender->AddUpdateOperation(new ChangeManipulator(m_pView,m_pSelfManipulator));
                    break;
                }
            }
        }
    }

    ///  更新视口
    if(m_bUpdateViewPort)
    {
        m_pView->getCamera()->setViewport(m_stViewPort.nX,m_stViewPort.nY,m_stViewPort.nWidth,m_stViewPort.nHeight);
        m_bUpdateViewPort = false;
    }

    /// 更新投影方式
    if(m_bUpdateProject)
    {
        double dAcesip = static_cast<double>(m_stViewPort.nWidth)/static_cast<double>(m_stViewPort.nHeight);
        switch (m_emProjectType)
        {
        case Ortho:
            m_pView->getCamera()->setProjectionMatrixAsOrtho2D(m_stViewPort.nX,m_stViewPort.nWidth,m_stViewPort.nY,m_stViewPort.nHeight);
            break;
        case Perspective:
            m_pView->getCamera()->setProjectionMatrixAsPerspective(45,dAcesip,0.01,100.);
            break;
        }
        m_bUpdateProject = false;
    }


}

void QtViewPort::HomeViewPoint()
{
    switch(m_emType)
    {
    case View_3DMap:
        m_pRender->AddUpdateOperation(new HomeViewPointOperation(m_p3DEarthManipulator));
        break;
    case View_2DMap:
        m_pRender->AddUpdateOperation(new HomeViewPointOperation(m_p2DEarthManipulator));
        break;
    case View_Node:
        m_pRender->AddUpdateOperation(new HomeViewPointOperation(m_pTrackManipulator));
        break;
    case View_Osg:
        m_pRender->AddUpdateOperation(new HomeViewPointOperation(m_pSelfManipulator));
        break;
    default:
        break;
    }
}

bool QtViewPort::SubMessage(IViewPortMessageObserver *pObserver)
{
    auto findResult = find(m_pAllOserver.begin(),m_pAllOserver.end(),pObserver);
    if(findResult == m_pAllOserver.end())
    {
        m_pAllOserver.push_back(pObserver);
    }

    return(true);
}

bool QtViewPort::UnSubMessage(IViewPortMessageObserver *pObserver)
{
    auto findResult = find(m_pAllOserver.begin(),m_pAllOserver.end(),pObserver);
    if(findResult != m_pAllOserver.end())
    {
        m_pAllOserver.erase(findResult);
        return(true);
    }
    else
    {
        return(false);
    }
}

/// 开始截图
void QtViewPort::BeginCapture()
{
    if(!m_pPostDrawBack.valid())
    {
        m_pPostDrawBack = new CaptureImageCallback(this);
        m_pView->getCamera()->addPostDrawCallback(m_pPostDrawBack);
    }
}

/// 停止截图
void QtViewPort::EndCapture()
{
    if(m_pPostDrawBack.valid())
    {
        m_pView->getCamera()->removePostDrawCallback(m_pPostDrawBack);
        m_pPostDrawBack=nullptr;
    }
}

/// 更新时间
void QtViewPort::UpdateTime(double dt)
{
}

#ifdef NEED_VR

/// 显示到VR上
bool QtViewPort::ShowOnVR()
{
    if(m_bOpenVR)
    {
        return(false);
    }

    /// 加载运行时
    vr::EVRInitError vrError = vr::VRInitError_None;
    m_pHMD = vr::VR_Init( &vrError,vr::VRApplication_Scene);

    if(vr::VRInitError_None != vrError)
    {
        OSG_WARN<<"VR_Init error: "
                <<vr::VR_GetVRInitErrorAsEnglishDescription(vrError)
                <<std::endl;
        return(false);
    }

    OSG_WARN<<"VR Init "
            << GetTrackedDeviceString(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String)
            << " "
            << GetTrackedDeviceString( vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SerialNumber_String)
            <<std::endl;

    m_pHMD->GetRecommendedRenderTargetSize(&m_nVrWidth,&m_nVrHeight);

    OSG_WARN<<m_nVrWidth<<'\t'<<m_nVrWidth<<std::endl;
    qDebug()<<m_nVrWidth<<'\t'<<m_nVrHeight;
    if (!vr::VRCompositor())
    {
        OSG_WARN<<"Compositor initialization failed"
                <<std::endl;

        ShutDownVR();
        return(false);
    }

    m_bOpenVR = true;
    m_bVRStatusChanged = true;

    return(true);
}

/// 关闭VR
bool QtViewPort::ShutDownVR()
{
    if(nullptr !=m_pHMD)
    {
        vr::VR_Shutdown();
        m_pHMD = nullptr;
        OSG_WARN<<"VR Close"<<std::endl;
    }

    m_bOpenVR = false;
    m_bVRStatusChanged = true;

    return(true);
}

/// 获取设备信息
std::string QtViewPort::GetTrackedDeviceString(vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError *peError)
{
    uint32_t unRequiredBufferLen = vr::VRSystem()->GetStringTrackedDeviceProperty( unDevice, prop, NULL, 0, peError );
    if(0 == unRequiredBufferLen)
    {
        return "";
    }

    char *pchBuffer = new char[ unRequiredBufferLen ];
    unRequiredBufferLen = vr::VRSystem()->GetStringTrackedDeviceProperty( unDevice, prop, pchBuffer, unRequiredBufferLen, peError );
    std::string sResult = pchBuffer;
    delete [] pchBuffer;
    return sResult;
}

#endif

/// 视点位置
void QtViewPort::EyePos(double dX, double dY, double dZ)
{
    static ScenePos s_stWordPos;
    auto iter = m_pAllOserver.begin();
    auto tmpIter = iter;

    s_stWordPos.fX = dX;
    s_stWordPos.fY = dY;
    s_stWordPos.fZ = dZ;

    for(;iter != m_pAllOserver.end();)
    {
        ++tmpIter;
        (*iter)->EypePos(s_stWordPos);
        iter = tmpIter;
    }
}

/// 查看的位置
void QtViewPort::LookDir(double dX, double dY, double dZ)
{
    static ScenePos s_stWordPos;
    auto iter = m_pAllOserver.begin();
    auto tmpIter = iter;

    s_stWordPos.fX = dX;
    s_stWordPos.fY = dY;
    s_stWordPos.fZ = dZ;

    for(;iter != m_pAllOserver.end();)
    {
        ++tmpIter;
        (*iter)->LookDir(s_stWordPos);
        iter = tmpIter;
    }
}

void QtViewPort::CaptureImage(int nWidht, int nHeight, QByteArray info)
{
    auto iter = m_pAllOserver.begin();
    auto tmpIter = iter;

    RGBAData* pData=new RGBAData;
    pData->unWidth = nWidht;
    pData->unHeight = nHeight;
    pData->pRGBAData=reinterpret_cast<unsigned char*>(info.data());
    for(;iter != m_pAllOserver.end();)
    {
        ++tmpIter;
        (*iter)->CaptureImage(pData);
        iter = tmpIter;
    }
    pData->pRGBAData=nullptr;
    delete pData;
}

/// 移除从相机
void QtViewPort::RemoveSlave()
{
    if(m_listStereoCamera.size() > 0)
    {
        m_pView->getCamera()->setGraphicsContext(m_listStereoCamera.front()->getGraphicsContext());
        for(auto one : m_listStereoCamera)
        {
            m_pView->removeSlave(m_pView->findSlaveIndexForCamera(one));
        }
        m_listStereoCamera.clear();
    }
}

void QtViewPort::UpdateViewPoint(CMyEarthManipulator *pEarthManipulator)
{
    auto ViewPoint = pEarthManipulator->getViewpoint();
    m_stViewPoint.fAzimuth = ViewPoint.heading()->as(osgEarth::Units::DEGREES);
    m_stViewPoint.fElev = -ViewPoint.pitch()->as(osgEarth::Units::DEGREES);
    m_stViewPoint.fDistance=ViewPoint.range()->as(osgEarth::Units::METERS);
    m_stViewPoint.stPos.fX = ViewPoint.focalPoint()->x();
    m_stViewPoint.stPos.fY = ViewPoint.focalPoint()->y();
    m_stViewPoint.stPos.fZ = ViewPoint.focalPoint()->z();
}

void QtViewPort::UpdateViewPoint(osgGA::OrbitManipulator *pManipulator)
{
    const osg::Vec3d& rCenter = pManipulator->getCenter();
    m_stViewPoint.stPos.fX = rCenter.x();
    m_stViewPoint.stPos.fY = rCenter.y();
    m_stViewPoint.stPos.fZ = rCenter.z();
    m_stViewPoint.fAzimuth = osg::RadiansToDegrees(pManipulator->getHeading());
    m_stViewPoint.fElev = osg::RadiansToDegrees(pManipulator->getElevation());
    m_stViewPoint.fDistance = pManipulator->getDistance();
}
