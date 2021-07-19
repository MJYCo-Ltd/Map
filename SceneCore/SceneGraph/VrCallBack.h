#include <openvr/openvr.h>

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