#ifndef INTERFACE_SCENE_SCALE_GROUP_HEARDER_H
#define INTERFACE_SCENE_SCALE_GROUP_HEARDER_H

#include <SceneGraph/ISceneGroup.h>
/**
 * @brief 缩放节点类
 */
class ISceneScaleGroup:public ISceneGroup
{
public:
    CONSTRUCTOR(ISceneScaleGroup,ISceneGroup)

    /**
     * @brief 转换成缩放节点
     * @return
     */
    ISceneScaleGroup* AsSceneScaleGroup(){return(this);}

    /**
     * @brief 设置缩放系数
     */
    void SetScal(double dScalBit)JUDGE_POSITIVE_DOUBLE_CALL_FUNCTION(dScalBit,m_dScalBit,ScalChanged)
    double Scal()const{return(m_dScalBit);}

    /**
     * @brief 是否开启自动缩放
     * @param bAutoScal
     */
    void SetAutoScal(bool bAutoScal)JUDGE_EQUAL_CALL_FUNCTION(bAutoScal,m_bAutoScal,AutoScalChanged)
    bool AutoScal(){return(m_bAutoScal);}

    /**
     * @brief 设置最小缩放倍数
     * @param dMinScal
     */
    void SetMinScal(double dMinScal)JUDGE_POSITIVE_DOUBLE_CALL_FUNCTION(dMinScal,m_dMinScal,MinScalChanged)
    double MinScal(){return(m_dMinScal);}

    /**
     * @brief 设置最大缩放倍数
     * @param dMaxScal
     */
    void SetMaxScal(double dMaxScal)JUDGE_POSITIVE_DOUBLE_CALL_FUNCTION(dMaxScal,m_dMaxScal,MaxScalChanged)
    double MaxScal(){return(m_dMaxScal);}
protected:
    virtual ~ISceneScaleGroup(){}
    virtual void ScalChanged()=0;
    virtual void AutoScalChanged()=0;
    virtual void MinScalChanged()=0;
    virtual void MaxScalChanged()=0;

protected:
    double        m_dScalBit{1.};
    double        m_dMinScal{};
    double        m_dMaxScal{DBL_MAX};
    bool          m_bAutoScal{false};
};

#endif//INTERFACE_SCENE_AUTO_SCALE_GROUP_HEARDER_H
