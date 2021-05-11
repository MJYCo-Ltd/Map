#include <Inner/OsgExtern/MapNodeCullBack.h>
#include <osgUtil/CullVisitor>
#include <osgEarth/MapNode>
#include <osgEarth/Terrain>
#include <osgEarth/LogarithmicDepthBuffer>
#include <VersionMathCommon.h>
#include <SceneGraph/IWindow.h>
#include <Inner/IOsgViewPoint.h>

CMapNodeCullBack::CMapNodeCullBack(ISceneGraph*pSceneGraph):m_pSceneGraph(pSceneGraph)
{
    m_pUserData = new YtyUserData;
    m_pPicker=new osgUtil::LineSegmentIntersector(osgUtil::Intersector::MODEL, m_stStartVertex, m_stEndVertex);
    m_pPicker->setIntersectionLimit( osgUtil::Intersector::LIMIT_NEAREST );
    m_pSceneGraph->GetMainWindow()->GetMainViewPoint()->SubMessage(this);
    m_pView=m_pSceneGraph->GetMainWindow()->GetMainViewPoint()->AsOsgViewPoint()->GetOsgView();
}

bool CMapNodeCullBack::run(osg::Object *object, osg::Object *data)
{
    static osgEarth::LogarithmicDepthBuffer s_logDepthBuffer;

    osg::Node* node = object ? object->asNode() : 0;

    if(nullptr == node->getUserData())
    {
        node->setUserData(m_pUserData);
    }

    if (nullptr != node)
    {
        auto pMapNode = static_cast<osgEarth::MapNode*>(node);
        if(nullptr != pMapNode && pMapNode->isGeocentric())
        {
            if(m_dEyeLength - R_Earth < 3000)
            {
                if(!m_bInstelld)
                {
                    s_logDepthBuffer.install(pMapNode);
                    m_bInstelld=true;
                }
            }
            else
            {
                if(m_bInstelld)
                {
                    s_logDepthBuffer.uninstall(pMapNode);
                    m_bInstelld=false;
                }
            }
        }

        osg::Viewport* pViewPort = m_pView->getCamera()->getViewport();
        static osg::Vec3d world;
        static osgEarth::GeoPoint geoPoint;

        static float local_x(0.f), local_y(0.0f);
        const osg::Camera* camera = m_pView->getCameraContainingPosition(m_nX, pViewPort ? pViewPort->height() - m_nY : m_nY,
                                                                         local_x, local_y);
        if (!camera)
            camera = m_pView->getCamera();
        // Build a matrix that transforms from the terrain/world space
        // to either clip or window space, depending on whether we have
        // a viewport. Is it even possible to not have a viewport? -gw
        osg::Matrixd matrix;

        // compensate for any transforms applied between terrain and camera:
        osg::Matrix terrainRefFrame = osg::computeLocalToWorld(pMapNode->getParentalNodePaths()[0]);
        matrix.postMult(terrainRefFrame);

        matrix.postMult(camera->getViewMatrix());
        matrix.postMult(camera->getProjectionMatrix());

        double zNear = -1.0;
        double zFar = 1.0;
        if (camera->getViewport())
        {
            matrix.postMult(camera->getViewport()->computeWindowMatrix());
            zNear = 0.0, zFar = 1.0;
        }

        static osg::Matrixd inverse;
        inverse.invert(matrix);

        static osg::Vec3d vDir, vUp;
        camera->getViewMatrixAsLookAt(m_stStartVertex, vDir, vUp);
        m_stEndVertex = osg::Vec3d(local_x,local_y,zFar) * inverse;

        m_pPicker->reset();
        m_pPicker->setStart(m_stStartVertex);
        m_pPicker->setEnd(m_stEndVertex);

        osgUtil::IntersectionVisitor iv(m_pPicker);
        iv.setTraversalMask(PICK_MASK);
        pMapNode->accept(iv);

        if (m_pPicker->containsIntersections())
        {
            world = m_pPicker->getIntersections().begin()->getWorldIntersectPoint();
            geoPoint.fromWorld(pMapNode->getMapSRS(),world);
            geoPoint.makeGeographic();
            m_pUserData->SetValue(geoPoint.x(),geoPoint.y(),geoPoint.z());
        }
    }

    return traverse(object, data);
}

/// 眼睛位置
void CMapNodeCullBack::EypePos(const ScenePos & eyePos)
{
    m_vEyePos.Set(eyePos.fX,eyePos.fY,eyePos.fZ);
    m_dEyeLength = m_vEyePos.Length();
}

///设置鼠标位置
void CMapNodeCullBack::SetMousePos(int nX, int nY)
{
    m_nX = nX;
    m_nY = nY;
}

const Math::CVector&CMapNodeCullBack::GetEyePos()const
{
    return (m_vEyePos);
}

CMapNodeCullBack::~CMapNodeCullBack()
{
    m_pSceneGraph->GetMainWindow()->GetMainViewPoint()->UnSubMessage(this);
}

void YtyUserData::SetValue(const double &dX, const double &dY, const double &dZ)
{
    m_dX=dX;
    m_dY=dY;
    m_dZ=dZ;
}
