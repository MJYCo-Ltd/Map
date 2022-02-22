#ifndef INTERFACE_SCENE_DONUT_HEARDER_H
#define INTERFACE_SCENE_DONUT_HEARDER_H
#include "ISensor.h"

/**
 * @brief 甜甜圈
 */
class ISceneDonut:public ISensor
{
public:
    ISceneDonut(ISceneGraph* pSceneGraph):ISensor(pSceneGraph){}
    /**
     * @brief 设内半径
     */
    void SetMinRadius(double dRadius)JUDGE_DOUBLE_CALL_FUNCTION(dRadius,m_dRMin,ShapeChanged)
	/**
     * @brief 设外半径
     */
    void SetMaxRadius(double dRadius)JUDGE_DOUBLE_CALL_FUNCTION(dRadius,m_dRMax,ShapeChanged)

protected:
    virtual ~ISceneDonut(){}
protected:
    double  m_dRMin{100000.0};
    double  m_dRMax{5.};
};

#endif
