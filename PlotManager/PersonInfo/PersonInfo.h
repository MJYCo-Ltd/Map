#ifndef CPERSONINFO_H
#define CPERSONINFO_H

#include <osgEarth/PlaceNode>

#include <Map/Plot/IPersonInfo.h>
#include <Inner/QtOsgEarthMapSceneNode.h>

#include "PersonInfo_Global.h"

class PersonInfoCallBack;

class CPersonInfo:public QtOsgEarthMapSceneNode<IPersonInfo>
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

    /**
     * @brief 设置分组类型
     * @param type
     */
    void SetType(GroupType type);
    GroupType GetType(){return(m_eGroup);}

    /**
     * @brief 设置人员状态
     * @param status
     */
    void SetStatus(PersonStatus status);
    PersonStatus GetStatus(){return(m_eStatus);}

    /**
     * @brief 获取接口名称
     * @return
     */
    static const string& GetInterFaceName(){return(S_sInterFace);}
protected:
    void changeImage();
private:
    osg::ref_ptr<osgEarth::PlaceNode> m_pPerson;
    osg::ref_ptr<PersonInfoCallBack>  m_pCallBack;   ///
    osgEarth::Style                   m_placeStyle;  /// 位置样式
    string                            m_sName;       ///  名字
    bool                              m_bUpdateImage=false;
    GroupType                         m_eGroup = NONE_GROUP;  /// 设置人员分组
    PersonStatus                      m_eStatus = PERSON_OK;  /// 设置人员状态
    static string                     S_sInterFace;  /// 接口名字
};

extern "C"
{
    PERSONINFOSHARED_EXPORT IPersonInfo* CreateNode(ISceneGraph*,const string& );
    PERSONINFOSHARED_EXPORT bool QueryInterface(string&);
//    PERSONINFOSHARED_EXPORT bool DeleteNode(IMapSceneNode*);
}

#endif // CPERSONINFO_H
