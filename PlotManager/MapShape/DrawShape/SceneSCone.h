#ifndef CSCENESCONE_H
#define CSCENESCONE_H
#include "Map/Plot/ISConeSensor.h"
#include <Inner/QtSensor.h>

class CSceneSCone:public QtSensor<ISConeSensor>
{
public:
    CSceneSCone(ISceneGraph*);

    /**
     * @brief 初始化节点
     */
    void InitSceneNode();

    void SetHAngle(double);
    double HAngle()const{return(m_dHAngle);}

    void SetVAngle(double);
    double VAngle() const{return(m_dVAngle);}
protected:
    void BuildGeometry();
protected:
    double m_dHAngle=1.0;
    double m_dVAngle=5.0;
};

#endif // CSCENESCONE_H
