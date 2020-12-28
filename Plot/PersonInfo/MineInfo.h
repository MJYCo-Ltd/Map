#ifndef CMINEINFO_H
#define CMINEINFO_H

#include <osgEarth/PlaceNode>

#include <Plot/PersonInfo/IMineInfo.h>
#include <Inner/ImplMapSceneNode.hpp>

#include <NoQt.h>

class CMineInfo:public ImplMapSceneNode<IMineInfo>
{
public:
    CONSTRUCTOR(CMineInfo,ImplMapSceneNode<IMineInfo>)
protected:
   /**
     * @brief 更新地图节点
     * @param pMapNode
     */
    void UpdateMapNode(osgEarth::MapNode* pMapNode);

    /**
      * @brief 初始化节点
      */
    void InitNode();

    /**
     * @brief 位置更新消息
     */
    void PosChanged();
    void NameChanged();
    void UpdateNode();
protected:
    bool m_bPosChanged=false;
    bool m_bNameChanged=false;
    osg::observer_ptr<osgEarth::PlaceNode> m_pMine;
    osgEarth::Style                   m_placeStyle;  /// 位置样式
};

#endif // CMINEINFO_H
