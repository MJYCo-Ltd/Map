#include <Inner/OsgExtern/OsgExtern.h>

RemoveFromeScene::RemoveFromeScene(osg::Node *pParent):
    m_pParent(pParent)
{
}

void RemoveFromeScene::operator()(osg::Object *)
{
    if(auto pGroup = m_pParent->asGroup())
    {
        pGroup->removeChildren(0,pGroup->getNumChildren());
    }

    auto parents = m_pParent->getParents();
    for(auto one : parents)
    {
        one->removeChild(m_pParent);
    }
}

/// 清空子节点
CClearChildNode::CClearChildNode(osg::Group* pParent):
    m_pParent(pParent)
{

}

CClearChildNode::~CClearChildNode()
{
}

void CClearChildNode::operator()(osg::Object*)
{
    m_pParent->removeChildren(0,m_pParent->getNumChildren());
}

/// 修改节点数据
CModifyNode::CModifyNode(osg::Group* pParent,
            osg::Node* pChild,
            bool bAdd):
    m_pParent(pParent),
    m_pChild(pChild),
    m_bAdd(bAdd)
{
}


CModifyNode::~CModifyNode()
{
}

void CModifyNode::operator()(osg::Object*)
{
    if(m_bAdd)
    {
        m_pParent->addChild(m_pChild);
    }
    else
    {
        m_pParent->removeChild(m_pChild);
    }
}
