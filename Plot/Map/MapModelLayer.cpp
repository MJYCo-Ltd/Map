#include <osg/PolygonOffset>
#include "MapModelLayer.h"

using namespace osgEarth;

CMapModelLayer::CMapModelLayer(ISceneGraph* pSceneGraph):
    ImplSceneGroup<ISceneGroup>(pSceneGraph)
{
}

void CMapModelLayer::init()
{
    VisibleLayer::init();
#if OSGEARTH_VERSION_LESS_OR_EQUAL(3,2,0)
    installDefaultOpacityShader();
#endif
}

void CMapModelLayer::InitNode()
{
    init();
    ImplSceneGroup<ISceneGroup>::InitNode();
}

osg::Node* CMapModelLayer::getNode() const
{
    return m_pGroup.get();
}

osg::Group* CMapModelLayer::getGroup() const
{
    return m_pGroup.get();
}
