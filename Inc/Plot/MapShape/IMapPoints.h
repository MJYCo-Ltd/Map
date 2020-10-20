#ifndef INTERFACE_MAP_POINTS_HEARDER_H
#define INTERFACE_MAP_POINTS_HEARDER_H
#include <list>
#include <Map/IMapSceneNode.h>
/**
 * @brief 场景节点类
 */
class IMapPoints:public IMapSceneNode
{
public:

    CONSTRUCTOR(IMapPoints,IMapSceneNode)

    /**
     * @brief 增加点
     */
    virtual void AddPoint(int,const MapGeoPos&)=0;

    /**
     * @brief 移除点
     */
    virtual bool RemovePoint(int)=0;

    /**
     * @brief 设置多个位置
     */
    virtual void SetMultPos(const std::vector<MapGeoPos>&)=0;
    const std::list<MapGeoPos>& GetMulPos(){return(m_listAllPos);}

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
    virtual ~IMapPoints(){}

    virtual void PosChanged()=0;
    virtual void ColorChanged()=0;
    virtual void PointSizeChanged()=0;

protected:
    std::list<MapGeoPos>   m_listAllPos;
    float                  m_fPointSize=1.f;
    SceneColor             m_stPointColor;
};

#endif//INTERFACE_MAP_POINTS_HEARDER_H
