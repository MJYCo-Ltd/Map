#include <osgEarth/AnnotationNode>
#include "MapModelLayer.h"

using namespace osgEarth;

void CMapModelLayer::init()
{
    VisibleLayer::init();

    _root = new osg::Group();

    deserialize();

    installDefaultOpacityShader();
}

osg::Node* CMapModelLayer::getNode() const
{
    return _root.get();
}

osg::Group* CMapModelLayer::getGroup() const
{
    return _root.get();
}

CMapModelLayer::CMapModelLayer()
{
    init();
}


void CMapModelLayer::addChild(AnnotationNode* node)
{
    _root->addChild(node);
}

void CMapModelLayer::deserialize()
{
    _root->removeChildren(0, _root->getNumChildren());
}
