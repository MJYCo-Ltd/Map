#ifndef QT_SENSOR_HEADER_H
#define QT_SENSOR_HEADER_H

#include "QtDrawShape.h"

template <typename T>
class QtSensor:public QtDrawShape<T>
{
public:
    explicit QtSensor(ISceneGraph* pSceneGraph):QtDrawShape<T>(pSceneGraph){}

    /**
     * @brief
     * @param rColor
     */
    void SetColor(const SceneColor& rColor)
    {
        if(m_stColor != rColor)
        {
            m_stColor = rColor;
            UpdateColor();
        }
    }

    const SceneColor& Color() const
    {
        return(m_stColor);
    }

    /**
     * @brief SetDistance
     * @param dDistance
     */
    void SetDistance(double dDistance)
    {
        if(dDistance > 0 && fabs(m_dDistance - dDistance) > 1e-10)
        {
            m_dDistance = dDistance;
            UpdateDistance();
        }
    }

    double Distance() const
    {
        return(m_dDistance);
    }
protected:
    virtual void UpdateColor(){}
    virtual void UpdateDistance(){}
protected:
    SceneColor m_stColor;
    double     m_dDistance=0.;
};

#endif
