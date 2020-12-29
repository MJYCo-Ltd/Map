#ifndef INTERFACE_PERSONINFO_HEARDER_H
#define INTERFACE_PERSONINFO_HEARDER_H
#include <string>

#include <Plot/Map/IMapSceneNode.h>

/**
 * @brief 分组类型
 */
enum GroupType
{
    RED_GROUP,
    BLUE_GROUP,
    NONE_GROUP
};

/**
 * @brief 人员状态
 */
enum PersonStatus
{
    PERSON_OK,
    PERSON_HURT,
    PERSON_DATH,
    PERSON_HIT,
    PERSON_UNLINE
};

/**
 * @brief 场景节点类
 */
class IPersonInfo:public IMapSceneNode
{
public:
    CONSTRUCTOR(IPersonInfo,IMapSceneNode)
    /**
     * @brief 设置名字
     */
    void SetName(const std::string& rName)JUDGE_EQUAL_CALL_FUNCTION(rName,m_sName,NameChanged)
    const std::string& GetName()const{return(m_sName);}

    /**
     * @brief 设置分组
     * @param type
     */
    void SetType(GroupType type)JUDGE_EQUAL_CALL_FUNCTION(type,m_emGroupType,GroupChanged)
    GroupType GetType()const{return(m_emGroupType);}

    /**
     * @brief 设置人员状态
     * @param status
     */
    void SetStatus(PersonStatus status)JUDGE_EQUAL_CALL_FUNCTION(status,m_emPersonStatus,StatusChanged)
    PersonStatus GetStatus()const{return(m_emPersonStatus);}

    /**
     * @brief 设置人员位置
     * @param rGeoPos
     */
    void SetPos(const MapGeoPos& rGeoPos)JUDGE_EQUAL_CALL_FUNCTION(rGeoPos,m_geoPos,PosChanged)
    const MapGeoPos& GetPos()const{return(m_geoPos);}
protected:
    virtual void NameChanged()=0;
    virtual void GroupChanged()=0;
    virtual void StatusChanged()=0;
    virtual void PosChanged()=0;

    virtual ~IPersonInfo(){}
protected:
    std::string m_sName;
    GroupType    m_emGroupType=NONE_GROUP;
    PersonStatus m_emPersonStatus=PERSON_OK;
    MapGeoPos    m_geoPos;
};

#endif
