#ifndef INTERFACE_PERSONINFO_HEARDER_H
#define INTERFACE_PERSONINFO_HEARDER_H
#include <string>

#include "../IMapSceneNode.h"
using namespace std;

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
    /**
     * @brief 设置名字
     */
    virtual void SetName(const string& )=0;
    virtual const string& GetName()=0;

    /**
     * @brief 设置分组
     * @param type
     */
    virtual void SetType(GroupType type)=0;
    virtual GroupType GetType()=0;

    /**
     * @brief 设置人员状态
     * @param status
     */
    virtual void SetStatus(PersonStatus status)=0;
    virtual PersonStatus GetStatus()=0;

};

#endif
