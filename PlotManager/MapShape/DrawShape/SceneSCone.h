#ifndef CSCENESCONE_H
#define CSCENESCONE_H
#include "Map/Plot/ISConeSensor.h"
#include <Inner/QtSensor.h>
class SConeCallback;

class CSceneSCone:public QtSensor<ISConeSensor>
{
    friend class SConeCallback;
public:
    CSceneSCone(ISceneGraph*);

    /**
     * @brief 初始化节点
     */
    void InitSceneNode();

    /**
     * @brief 设置运动垂向半角
     */
    void SetHAngle(double);
    double HAngle()const{return(m_dHAngle);}

    /**
     * @brief 设置运动方向上半角
     */
    void SetVAngle(double);
    double VAngle() const{return(m_dVAngle);}
protected:
    void BuildGeometry();

    void PosChanged();
protected:
    double m_dHAngle=0.0;
    double m_dVAngle=0.0;
};

#endif // CSCENESCONE_H
