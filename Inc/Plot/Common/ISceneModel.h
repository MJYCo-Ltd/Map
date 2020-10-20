#ifndef INTERFACE_SCENE_MODEL_HEARDER_H
#define INTERFACE_SCENE_MODEL_HEARDER_H

#include <limits>
#include <SceneGraph/ISceneNode.h>
/**
 * @brief 场景模型节点类
 */
class ISceneModel:public ISceneNode
{
public:
    CONSTRUCTOR(ISceneModel,ISceneNode)

    /**
     * @brief 转换成模型节点
     * @return
     */
    ISceneModel* AsSceneModel(){return(this);}

    /**
     * @brief 设置模型初始化的姿态
     * @param rAttitude
     */
    void InitAttitude(const SceneAttitude& rAttitude)JUDGE_EQUAL_CALL_FUNCTION(rAttitude,m_stInitAttitude,InitAttitudeChanged)
    const SceneAttitude& InitAttitude()const{return(m_stInitAttitude);}
protected:
    virtual ~ISceneModel(){}
    virtual void InitAttitudeChanged()=0;

protected:
    SceneAttitude m_stInitAttitude;
};

#endif
