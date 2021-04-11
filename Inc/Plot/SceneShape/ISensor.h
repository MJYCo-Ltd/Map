#ifndef INTERFACE_SENSOR_HEARDER_H
#define INTERFACE_SENSOR_HEARDER_H
#include <Plot/SceneShape/IShape.h>

/**
 * @brief 传感器基类
 */
class ISensor:public IShape
{
public:
    enum EffectType
    {
        UNIFORM_MOTION,//匀速运动
        NO_EFFECTE     //没有特效
    };
    CONSTRUCTOR(ISensor,IShape)

    /**
     * @brief 是否显示背面
     * @param bShow
     */
    void SetShowBack(bool bShow)JUDGE_EQUAL_CALL_FUNCTION(bShow,m_bShowBack,ShowBackChanged)
    bool ShowBack()const{return(m_bShowBack);}

    /**
     * @brief 显示面
     */
    void ShowFace(bool bShow)JUDGE_EQUAL_CALL_FUNCTION(bShow,m_bShowFace,ShowTypeChanged)
    bool IsShowFace()const{return(m_bShowFace);}

    /**
     * @brief 显示线
     */
    void ShowLine(bool bShow)JUDGE_EQUAL_CALL_FUNCTION(bShow,m_bShowLine,ShowTypeChanged)
    bool IsShowLine()const{return(m_bShowLine);}

    /**
     * @brief 设置探测距离
     */
    void SetDistance(double dDistance)JUDGE_DOUBLE_CALL_FUNCTION(dDistance,m_dDistance,DistanceChanged)
    double Distance()const{return(m_dDistance);}

    /**
     * @brief 设置特效模式
     */
    void SetEffects(EffectType type)JUDGE_EQUAL_CALL_FUNCTION(type,m_emEffect,EffectsChanged)
    const EffectType Effects()const{return(m_emEffect);}

    /**
     * @brief 设置波的个数模式
     */
    void SetCount(unsigned short unCount)JUDGE_EQUAL_CALL_FUNCTION(unCount,m_unCount,CountChanged)
    unsigned short Count()const{return(m_unCount);}

    /**
     * @brief 设置波束的方向是不是向外走
     */
    void SetOut(bool bOut)JUDGE_EQUAL_CALL_FUNCTION(bOut,m_bOut,DirectionChanged)
    bool IsOut()const{return (m_bOut);}

    /**
     * @brief 设置波束的频率
     */
    void SetFreq(unsigned short unFreq)JUDGE_EQUAL_CALL_FUNCTION(unFreq,m_unFreq,FreqChanged)
    unsigned short Freq()const{return(m_unFreq);}

protected:
    virtual ~ISensor(){}

    virtual void DistanceChanged()=0;
    virtual void ShowTypeChanged()=0;
    virtual void EffectsChanged()=0;
    virtual void CountChanged()=0;
    virtual void DirectionChanged()=0;
    virtual void FreqChanged()=0;
    virtual void ShowBackChanged()=0;
protected:
    bool       m_bShowLine=true;
    bool       m_bShowFace=true;
    bool       m_bShowBack=true;
    double     m_dDistance=1.;
    unsigned short m_unCount=10u;
    unsigned short m_unFreq=1u;
    bool       m_bOut=true;
    EffectType m_emEffect=NO_EFFECTE;
};

#endif
