#ifndef CMINEINFO_H
#define CMINEINFO_H

#include <osgEarth/PlaceNode>

#include <Plot/PersonInfo/IMineInfo.h>
#include <Inner/ImplMapSceneNode.hpp>

#include <NoQt.h>
class IMapLocation;
class ILabel;
class IImage;
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
    void ColorChanged();
    void TypeChanged();
protected:
    bool m_bPosChanged=false;
    bool m_bNameChanged=false;
    bool m_bColorChanged=false;
    IMapLocation* m_pLocation=nullptr;
    ILabel*       m_pLabel=nullptr;
    IImage*       m_pImage=nullptr;
};

#endif // CMINEINFO_H
