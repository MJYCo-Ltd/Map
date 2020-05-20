#ifndef CPERSONINFO_H
#define CPERSONINFO_H

#include <osgEarth/PlaceNode>

#include <Map/Plot/IPersonInfo.h>
#include <Inner/QtSceneNode.h>

#include "PersonInfo_Global.h"

class PersonInfoCallBack;

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
     * @brief 位置更新消息
     */
    void PosChanged();

    /**
     * @brief 设置人员名字
     * @param strName
     */
    void SetName(const string& strName);
    const string& GetName();
private:
    osg::ref_ptr<osgEarth::PlaceNode> m_pPerson;
    osg::ref_ptr<PersonInfoCallBack>   m_pCallBack;  ///
    osgEarth::Symbology::Style        m_placeStyle; /// 位置样式
    string                            m_sName;      ///  名字
};

extern "C"
{
    PERSONINFOSHARED_EXPORT IPersonInfo* CreateNode(ISceneGraph*pSceneGraph);
}

#endif // CPERSONINFO_H
