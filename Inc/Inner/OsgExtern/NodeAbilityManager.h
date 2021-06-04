#ifndef SPLITRECTANGLE_HEADER_H
#define SPLITRECTANGLE_HEADER_H

#include <Inner/OsgExtern/OsgExtern_Global.h>
#include <SceneGraph/SceneType.h>
#include <SceneGraph/INodeAbility.h>

/**
 * @brief将矩形划分成点序列
 */
class OSGEXTERNSHARED_EXPORT CNodeAbilityManager
{
public:
    static CNodeAbilityManager* GetInstance();

    /**
     * @brief 根据类型创建能力
     * @param emType 能力类型
     * @return 能力值
     */
    INodeAbility* CreateAbility(ABILITY_TYPE emType);

    /**
     * @brief 删除能力
     * @param 能力指针
     * @return
     */
    bool RemoveAbility(INodeAbility*& pNodeAbility);
protected:
    CNodeAbilityManager(){}
};

#endif // SPLITRECTANGLE_HEADER_H
