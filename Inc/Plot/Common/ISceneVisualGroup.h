#ifndef INTERFACE_SCENE_VISUAL_GROUP_HEARDER_H
#define INTERFACE_SCENE_VISUAL_GROUP_HEARDER_H

#include <SceneGraph/ISceneGroup.h>
#include <SceneGraph/ViewType.h>

/**
 * @brief 屏幕节点类
 */
class ISceneVisualGroup:public ISceneGroup
{
public:
    CONSTRUCTOR(ISceneVisualGroup,ISceneGroup)

    /**
     * @brief 转换成本类型
     * @return
     */
    ISceneVisualGroup* AsSceneVisualGroup(){return(this);}

    /**
     * @brief 设置观察张角
     * @param dFovy [deg][0~180]
     */
    void SetFovy(double dFovy)JUDGE_POSITIVE_DOUBLE_CALL_FUNCTION(dFovy,m_dFovy,ParameterChanged)
    double Fovy()const{return(m_dFovy);}

    /**
     * @brief 设置观察的纵横比
     */
    void SetAspect(double dAspect)JUDGE_POSITIVE_DOUBLE_CALL_FUNCTION(dAspect,m_dAspect,ParameterChanged)
    double Aspect()const{return(m_dAspect);}

    /**
     * @brief 设置分析位置
     * @attention rViewPint.stPos 位置为
     */
    void SetViewPoint(const SceneViewPoint& rViewPoint)JUDGE_EQUAL_CALL_FUNCTION(rViewPoint,m_stViewPoint,ParameterChanged)
    const SceneViewPoint& ViewPoint()const{return(m_stViewPoint);}
protected:
    virtual ~ISceneVisualGroup(){}
    virtual void ParameterChanged()=0;
protected:
    double m_dFovy=45.;   /// 张角
    double m_dAspect=1.;  /// 纵横比
    SceneViewPoint m_stViewPoint; ///视点位置
};

#endif //INTERFACE_SCENE_FLASH_GROUP_HEARDER_H
