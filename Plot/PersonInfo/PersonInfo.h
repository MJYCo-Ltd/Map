#ifndef CPERSONINFO_H
#define CPERSONINFO_H

#include <osgEarth/PlaceNode>

#include <Plot/PersonInfo/IPersonInfo.h>
#include <Inner/ImplMapSceneNode.hpp>

#include <NoQt.h>

class PersonInfoCallBack;

class CPersonInfo:public ImplMapSceneNode<IPersonInfo>
{
public:
    CONSTRUCTOR(CPersonInfo,ImplMapSceneNode<IPersonInfo>)
    ~CPersonInfo();

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
    void GroupChanged();
    void StatusChanged();

    void UpdateNode();
    void changeImage();
private:
    bool m_bPosChanged=false;
    bool m_bNameChanged=false;
    bool m_bGroupChanged=false;
    bool m_bStatusChanged=false;
    osg::observer_ptr<osgEarth::PlaceNode> m_pPerson;
    osgEarth::Style                   m_placeStyle;  /// 位置样式
};

extern "C"
{
    Q_DECL_EXPORT IPersonInfo* CreateNode(ISceneGraph*,const string& );
    Q_DECL_EXPORT bool QueryInterface(string&);
}

#endif // CPERSONINFO_H
