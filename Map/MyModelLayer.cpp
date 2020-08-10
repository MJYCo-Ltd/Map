#include <osgEarth/AnnotationNode>
#include "MyModelLayer.h"

using namespace osgEarth;

void CMyModelLayer::init()
{
    VisibleLayer::init();

    _root = new osg::Group();

    deserialize();

    installDefaultOpacityShader();
}

osg::Node* CMyModelLayer::getNode() const
{
    return _root.get();
}

osg::Group* CMyModelLayer::getGroup() const
{
    return _root.get();
}

CMyModelLayer::CMyModelLayer()
{
    init();
}


void CMyModelLayer::addChild(AnnotationNode* node)
{
    _root->addChild(node);
}

void CMyModelLayer::deserialize()
{
    _root->removeChildren(0, _root->getNumChildren());
}

