#ifndef CSCENEPULSE_H
#define CSCENEPULSE_H
#include "Map/Plot/IPulseSensor.h"
#include <Inner/QtSensor.h>

class CScenePulse:public QtSensor<IPulseSensor>
{
    friend class CPulseCallback;
public:
    CScenePulse(ISceneGraph*);

    /**
     * @brief 设置开始半径
     */
    void SetStartRadius(const double&);
    double GetStartRadius() const{return(m_dSpaceRadius);}

    /**
     * @brief 设置最终半径
     */
    void SetEndRadius(const double&);
    double GetEndRadius()const{return(m_dEndRadius);}

    void SetPulseCount(const int&);

    /**
     * @brief 设置终点位置
     */
    void SetEndPos(const ScenePos&);

    /**
     * @brief 初始化节点
     */
    void InitSceneNode();
protected:
    void PosChanged();
private:
    double     m_dStartRadius=0;      /// 设置开始半径
    double     m_dEndRadius=0;        /// 设置结束半径
    double     m_dLength=0;              /// 脉冲波长度
    double     m_dSpaceRadius=0;    /// 间隔
    double     m_dSpace=0;                /// 脉冲波宽度
    double     m_dHalSpace;          /// 脉冲波半宽
    ScenePos   m_stEndPos;           /// 终点位置

    int         m_nPulseCount=1;     /// 脉冲数量
};

#endif // CSCENEPULSE_H
