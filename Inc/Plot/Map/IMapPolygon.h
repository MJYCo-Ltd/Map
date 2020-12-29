#ifndef INTERFACE_POLYGON_HEARDER_H
#define INTERFACE_POLYGON_HEARDER_H
#include <list>
#include <Plot/Map/IMapSceneNode.h>


/**
 * @brief 场景节点类
 */
class IMapPolygon:public IMapSceneNode
{
public:
    CONSTRUCTOR(IMapPolygon,IMapSceneNode)

    /**
     * @brief 增加点
     */
    virtual void AddPoint(int,const MapGeoPos&)=0;

    /**
     * @brief 移除点
     */
    virtual bool RemovePoint(int)=0;

    /**
     * @brief 更新指定位置点
     */
    virtual bool UpdatePoint(int,const MapGeoPos&)=0;

    /**
     * @brief 设置多个位置
     */
    virtual void SetMultPos(const std::vector<MapGeoPos>&)=0;
    const std::list<MapGeoPos>& GetMulPos(){return(m_listAllPos);}

    /**
     * @brief 设置点的大小
     */
    void SetPolygonColor(const SceneColor& polygonColor)JUDGE_EQUAL_CALL_FUNCTION(polygonColor,m_stPolygonColor,PolygonColorChanged)
    const SceneColor& PolygonColor(){return(m_stPolygonColor);}
protected:
    virtual ~IMapPolygon(){}

    virtual void PolygonColorChanged()=0;
protected:
    SceneColor             m_stPolygonColor;
    std::list<MapGeoPos>   m_listAllPos;
};

#endif
