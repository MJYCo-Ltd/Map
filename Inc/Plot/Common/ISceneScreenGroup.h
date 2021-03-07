#ifndef INTERFACE_SCENE_SCREEN_GROUP_HEARDER_H
#define INTERFACE_SCENE_SCREEN_GROUP_HEARDER_H

#include <SceneGraph/ISceneGroup.h>

/**
 * @brief 屏幕节点类
 */
class ISceneScreenGroup:public ISceneGroup
{
public:
    CONSTRUCTOR(ISceneScreenGroup,ISceneGroup)

    /**
     * @brief 转换成本类型
     * @return
     */
    ISceneScreenGroup* AsSceneScreenGroup(){return(this);}

    /**
     * @brief 设置标牌是否避让
     * @return
     */
    void SetAvoid(bool bAvoid)JUDGE_EQUAL_CALL_FUNCTION(bAvoid,m_bAvoid,AvoidChanged)
    bool IfAvoid() const {return(m_bAvoid);}
protected:
    virtual ~ISceneScreenGroup(){}
    virtual void AvoidChanged()=0;
protected:
    bool m_bAvoid=false;
};

#endif //INTERFACE_SCENE_FLASH_GROUP_HEARDER_H
