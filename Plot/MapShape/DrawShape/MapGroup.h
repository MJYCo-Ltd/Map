#ifndef MAPGROUP_H
#define MAPGROUP_H

#include <Plot/Map/IMapGroup.h>
#include <Inner/ImplMapSceneNode.hpp>
class CMapGroup :public ImplMapSceneNode<IMapGroup>
{
public:
    CONSTRUCTOR(CMapGroup,ImplMapSceneNode<IMapGroup>)
    /**
     * @brief
     * @param pNode
     */
    void AddSceneNode(ISceneNode* pNode);

    virtual void RemoveSceneNode(ISceneNode* pNode);
    /**
     * @brief 更新地图节点
     * @param pMapNode
     */
    void Update();

protected:
    /**
     * @brief UpdateNode
     */
    void FrameCall();

    /**
     * @brief 初始化节点
     */
    void InitNode();
protected:
    osg::observer_ptr<osg::Group>  m_pGroup;
};

#endif // MAPGROUP_H
