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
        if(pMapNode->getTerrain()->getWorldCoordsUnderMouse(m_pView.get(), m_nX, pViewPort ? pViewPort->height() - m_nY : m_nY, world))
        {
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
