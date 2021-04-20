#include <Inner/OsgExtern/MapNodeCullBack.h>
#include <osgUtil/CullVisitor>
#include <osgEarth/MapNode>
#include <osgEarth/Terrain>
#include <osgEarth/LogarithmicDepthBuffer>

CMapNodeCullBack::CMapNodeCullBack()
{
//    m_pCullBack = new osgEarth::Util::AutoClipPlaneCullCallback;
}

bool CMapNodeCullBack::run(osg::Object *object, osg::Object *data)
{
    static osgEarth::LogarithmicDepthBuffer s_logDepthBuffer;

    osg::Node* node = object ? object->asNode() : 0;
    osg::NodeVisitor* nv = data ? data->asNodeVisitor() : 0;

    if (nullptr != node && nullptr != nv)
    {
        osgUtil::CullVisitor* cv = nv->asCullVisitor();
        if(nullptr != cv)
        {
            auto pMapNode = dynamic_cast<osgEarth::MapNode*>(node);
            if(nullptr != pMapNode)
            {
                osg::Vec3d eye, center, up;
                cv->getModelViewMatrix()->getLookAt( eye, center, up );

                osgEarth::GeoPoint loc;
                if ( pMapNode )
                {
                    loc.fromWorld( pMapNode->getMapSRS(), eye );
                }
                else
                {
                    static osg::EllipsoidModel em;
                    osg::Vec3d t;
                    em.convertXYZToLatLongHeight( eye.x(), eye.y(), eye.z(), loc.y(), loc.x(), loc.z() );
                }

                //double hae = loc.z();
                double hae = loc.z();
                if(pMapNode->getTerrain())
                {
                    double height = 0.0;
                    pMapNode->getTerrain()->getHeight(loc.getSRS(), loc.x(), loc.y(), &height);

                    hae -= height;
                }

                if(hae < 2000)
                {
                    if(!m_bInstelld)
                    {
//                        pMapNode->addCullCallback(m_pCullBack);
                        s_logDepthBuffer.install(pMapNode);
                        m_bInstelld=true;
                    }
                }
                else
                {
                    if(m_bInstelld)
                    {
                        s_logDepthBuffer.uninstall(pMapNode);
//                        pMapNode->removeCullCallback(m_pCullBack);
                        m_bInstelld=false;
                    }
                }
            }
        }
    }

    return traverse(object, data);
}
