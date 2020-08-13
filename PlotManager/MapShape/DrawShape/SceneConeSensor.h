#ifndef HEADER_CSCENECONE_H
#define HEADER_CSCENECONE_H
#include "Map/Plot/IConeSensor.h"
#include <Inner/QtSensor.h>
class CSceneCone:public QtSensor<IConeSensor>
{
public:
    CSceneCone(ISceneGraph*);
};

#endif // CSCENECONE_H
