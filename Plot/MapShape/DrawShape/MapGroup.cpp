#include "MapGroup.h"

void CMapGroup::AddSceneNode(ISceneNode *pNode)
{
    AddNode(m_pGroup.get(),pNode->AsOsgSceneNode()->GetRealNode());
}

void CMapGroup::RemoveSceneNode(ISceneNode *pNode)
{
    DelNode(m_pGroup.get(),pNode->AsOsgSceneNode()->GetRealNode());
}

void CMapGroup::Update()
{
    ImplMapSceneNode<IMapGroup>::UpdateMapNode();

}
void CMapGroup::FrameCall()
{
    ImplMapSceneNode<IMapGroup>::FrameCall();
}
/// 初始化节点
void CMapGroup::InitNode()
{
    ImplMapSceneNode<IMapGroup>::InitNode();
    m_pGroup = new osg::Group;
    ImplMapSceneNode<IMapGroup>::SetMapSceneNode(m_pGroup.get());
}
