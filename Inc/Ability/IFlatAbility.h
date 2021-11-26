#ifndef INTERFACE_FLAT_ABILITY_HEARDER_H
#define INTERFACE_FLAT_ABILITY_HEARDER_H

#include <SceneGraph/SceneType.h>
#include <SceneGraph/INodeAbility.h>

/**
 * @brief 平整的区域
 */
struct FlatArea
{
    ScenePos stLeftUp;    /// 左上角的位置
    ScenePos stRightUp;   /// 右上角的位置
    ScenePos stRightDown; /// 右下角的位置
    ScenePos stLeftDown;  /// 左下角的位置
    bool operator == (const FlatArea& rOther) const
    {
        if(&rOther == this)
        {
            return(true);
        }

        return(rOther.stLeftUp == stLeftUp
         && rOther.stRightUp == stRightUp
         && rOther.stRightDown == stRightDown
         && rOther.stLeftDown == stLeftDown);
    }
};

/**
 * @brief 场景节点类
 */
class IFlatAbility:public INodeAbility
{
public:

    /**
     * @brief 转换成本类型
     * @return
     */
    IFlatAbility* AsFlatAbility(){return(this);}

    /**
     * @brief 添加洞
     * @return 洞的索引
     */
    virtual int AddHole(const FlatArea& flatArea)=0;
    
    /**
     * @brief 删除洞
     * @return
     */
    virtual bool RemoveHole(int nIndex)=0;
protected:
    virtual ~IFlatAbility(){}
};

#endif //INTERFACE_FLAT_ABILITY_HEARDER_H
