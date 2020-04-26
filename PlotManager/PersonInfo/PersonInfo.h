#ifndef CPERSONINFO_H
#define CPERSONINFO_H

#include <osgEarth/PlaceNode>

#include <Map/Plot/IPersonInfo.h>
#include <Inner/QtSceneNode.h>

#include "PersonInfo_Global.h"

class CPersonInfo:public QtSceneNode<IPersonInfo>
{
public:
    CPersonInfo(ISceneGraph*pSceneGraph);
    ~CPersonInfo();

    /**
     * @brief 更新地图节点
     * @param pMapNode
     */
    void UpdateMapNode(osgEarth::MapNode* pMapNode);

    /**
     * @brief 初始化节点
     */
    void InitSceneNode();

    /**
     * @brief 设置位置
     * @param pScenePos
     */
    void SetPos(const ScenePos& pScenePos);
    void Add();
    void Del();
private:
    osg::observer_ptr<osgEarth::PlaceNode> m_pPerson;
    osgEarth::Symbology::Style        m_placeStyle;        /// 位置样式
};

extern "C"
{
    PERSONINFOSHARED_EXPORT IPersonInfo* CreateNode(ISceneGraph*pSceneGraph);
}

#endif // CPERSONINFO_H
