#ifndef INTERFACE_PERSONINFO_HEARDER_H
#define INTERFACE_PERSONINFO_HEARDER_H
#include <string>

#include "../IMapSceneNode.h"
using namespace std;

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

};

#endif
