#ifndef CSCENEPULSE_H
#define CSCENEPULSE_H
#include <Plot/SceneShape/IPulseSensor.h>
#include <Inner/ImplSceneSensor.hpp>

class CScenePulse:public ImplSceneSensor<IPulseSensor>
{
public:
    CONSTRUCTOR(CScenePulse,ImplSceneSensor<IPulseSensor>)

protected:
    /**
     * @brief 初始化节点
     */
    void CreateShape();
    void UpdateShape();

private:
    double     m_dLength=0;         /// 脉冲波长度
    double     m_dSpaceRadius=0;    /// 间隔
    double     m_dSpace=0;          /// 脉冲波宽度
    double     m_dHalSpace;          /// 脉冲波半宽
};

#endif // CSCENEPULSE_H
