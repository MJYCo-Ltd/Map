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
    void SetScal(double dScalBit){if(dScalBit>0. && fabs(m_dScalBit - dScalBit)>1e-15){m_dScalBit=dScalBit;ScalChanged();}}
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
    void SetMinScal(double dMinScal){if(dMinScal>0. && fabs(m_dMinScal-dMinScal)>DBL_EPSILON){m_dMinScal=dMinScal;AutoScalChanged();}}
    double MinScal(){return(m_dMinScal);}

    /**
     * @brief 设置最大缩放倍数
     * @param dMaxScal
     */
    void SetMaxScal(double dMaxScal){if(dMaxScal>0. && fabs(m_dMaxScal-dMaxScal)>DBL_EPSILON){m_dMaxScal=dMaxScal;AutoScalChanged();}}
    double MaxScal(){return(m_dMaxScal);}
protected:
    virtual ~ISceneScaleGroup(){}
    virtual void ScalChanged()=0;
    virtual void AutoScalChanged()=0;

protected:
    double        m_dScalBit=1.;
    double        m_dMinScal=0.;
    double        m_dMaxScal=DBL_MAX;
    bool          m_bAutoScal=true;
};

#endif//INTERFACE_SCENE_AUTO_SCALE_GROUP_HEARDER_H
