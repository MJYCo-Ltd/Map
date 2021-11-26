#ifndef INTERFACE_NODE_ABILITY_HEARDER_H
#define INTERFACE_NODE_ABILITY_HEARDER_H

class IFlashAbility;
class IFlatAbility;
class ISceneNode;

/**
 * @brief 场景节点类
 */
class INodeAbility
{
public:
    /**
     * @brief 更新能力
     */
    virtual void UpdateAbility()=0;

    /**
     * @brief 绑定场景节点
     */
    virtual void BoundNode(ISceneNode*)=0;

    /**
     * @brief 转换成闪烁能力
     * @return
     */
    virtual IFlashAbility* AsFlashAbility(){return(nullptr);}
    virtual IFlatAbility* AsFlatAbility(){return(nullptr);}
};

#endif //INTERFACE_NODE_ABILITY_HEARDER_H
