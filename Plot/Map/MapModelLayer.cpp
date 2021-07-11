#include <osgEarth/AnnotationNode>
#include <osgEarth/DepthOffset>
#include "MapModelLayer.h"

using namespace osgEarth;

CMapModelLayer::CMapModelLayer(ISceneGraph* pSceneGraph):
    ImplSceneGroup<ISceneGroup>(pSceneGraph)
{
}

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
