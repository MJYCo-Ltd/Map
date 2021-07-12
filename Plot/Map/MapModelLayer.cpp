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

    installDefaultOpacityShader();
}

void CMapModelLayer::InitNode()
{
    init();
    ImplSceneGroup<ISceneGroup>::InitNode();

    m_pGroup->getOrCreateStateSet()->setAttributeAndModes(
                new osg::PolygonOffset(-1, -1), 1);

    m_pGroup->getOrCreateStateSet()->setAttributeAndModes(
                new osg::Depth(osg::Depth::LEQUAL, 0, 1, true));

}

osg::Node* CMapModelLayer::getNode() const
{
    return m_pGroup.get();
}

osg::Group* CMapModelLayer::getGroup() const
{
    return m_pGroup.get();
}
