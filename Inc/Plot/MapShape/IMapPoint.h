#ifndef INTERFACE_MAP_POINT_HEARDER_H
#define INTERFACE_MAP_POINT_HEARDER_H

#include <Map/IMapSceneNode.h>
/**
 * @brief 场景节点类
 */
class IMapPoint:public IMapSceneNode
{
public:

    CONSTRUCTOR(IMapPoint,IMapSceneNode)

    /**
     * @brief 设置点的位置
     * @param rGeoPos
     */
    void SetGeoPos(const MapGeoPos& rGeoPos)JUDGE_EQUAL_CALL_FUNCTION(rGeoPos,m_stGeoPos,PosChanged)
    const MapGeoPos& GeoPos(){return(m_stGeoPos);}

    /**
     * @brief 设置点的大小
     */
    void SetPointSize(float fPointSize){if(fPointSize > 0.f && fabs(fPointSize-m_fPointSize)>FLT_EPSILON){m_fPointSize=fPointSize;PointSizeChanged();}}
    const float& PointSize()const{return(m_fPointSize);}

    /**
     * @brief 设置点的大小
     */
    void SetPointColor(const SceneColor& rColor)JUDGE_EQUAL_CALL_FUNCTION(rColor,m_stPointColor,ColorChanged)
    const SceneColor& PointColor() const{return(m_stPointColor);}
protected:
    virtual ~IMapPoint(){}

    virtual void PosChanged()=0;
    virtual void ColorChanged()=0;
    virtual void PointSizeChanged()=0;

protected:
    float      m_fPointSize=1.f;
    MapGeoPos  m_stGeoPos;
    SceneColor m_stPointColor;
};

#endif
