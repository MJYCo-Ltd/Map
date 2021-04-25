#include <osgEarth/AnnotationNode>
#include "MapModelLayer.h"

using namespace osgEarth;

void CMapModelLayer::init()
{
    VisibleLayer::init();

    installDefaultOpacityShader();
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

CMapModelLayer::CMapModelLayer(ISceneGraph* pSceneGraph):
    ImplSceneGroup<ISceneGroup>(pSceneGraph)
{
}
