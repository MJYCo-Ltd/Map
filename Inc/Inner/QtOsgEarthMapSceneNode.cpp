#include "QtOsgEarthMapSceneNode.h"


template <typename T>
QtOsgEarthMapSceneNode<T>::QtOsgEarthMapSceneNode(ISceneGraph *pSceneGraph):
       QtOsgSceneNode<T>(pSceneGraph)
{
}

/// 析构函数
template <typename T>
QtOsgEarthMapSceneNode<T>::~QtOsgEarthMapSceneNode()
{
}

template<typename T>
void QtOsgEarthMapSceneNode<T>::UpdateMapNode(osgEarth::MapNode *pMapNode)
{
}
