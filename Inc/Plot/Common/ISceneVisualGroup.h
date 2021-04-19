#ifndef INTERFACE_SCENE_VISUAL_GROUP_HEARDER_H
#define INTERFACE_SCENE_VISUAL_GROUP_HEARDER_H

#include <SceneGraph/ISceneGroup.h>

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
     * @brief 设置可视距离
     */
    void SetDistance(double dDistance)JUDGE_POSITIVE_DOUBLE_CALL_FUNCTION(dDistance,m_dDistance,ParameterChanged)
    double Distance()const{return(m_dDistance);}
protected:
    virtual ~ISceneVisualGroup(){}
    virtual void ParameterChanged()=0;
protected:
    double m_dFovy=45.;   /// 张角
    double m_dAspect=1.;  /// 纵横比
    double m_dDistance=1.;/// 可视距离
};

#endif //INTERFACE_SCENE_FLASH_GROUP_HEARDER_H
