#ifndef QT_SENSOR_HEADER_H
#define QT_SENSOR_HEADER_H

#include "QtDrawShape.h"

template <typename T>
class QtSensor:public QtDrawShape<T>
{
public:
    explicit QtSensor(ISceneGraph* pSceneGraph):QtDrawShape<T>(pSceneGraph){}
};

#endif
