#ifndef HEADER_CSCENECONE_H
#define HEADER_CSCENECONE_H
#include "Map/Plot/IConeSensor.h"
#include <Inner/QtSensor.h>
class ConeCallback;

class CSceneCone:public QtSensor<IConeSensor>
{
    friend class ConeCallback;
public:
    CSceneCone(ISceneGraph*);

    /**
     * @brief 更新地图节点
     * @param pMapNode
     */
    void UpdateMapNode(osgEarth::MapNode* pMapNode);

    /**
     * @brief 初始化节点
     */
    void InitSceneNode();

    void SetAngle(double);
    double Angle()const
    {
        return(m_dAngle);
    }
protected:
    void BuildGeometry();

    void PosChanged();
    void UpdateColor();
    void UpdateDistance();
protected:
    double m_dAngle=0.;
    bool   m_bUpdate=false;
};

#endif // CSCENECONE_H
