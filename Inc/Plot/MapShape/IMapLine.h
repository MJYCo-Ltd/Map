#ifndef INTERFACE_MAP_LINE_HEARDER_H
#define INTERFACE_MAP_LINE_HEARDER_H
#include <list>
#include <Map/IMapSceneNode.h>


enum MAP_LINE_TYPE
{
    MAP_SOLID_LINE, /// 实线
    MAP_DOTTED_LINE /// 虚线
};

/**
 * @brief 场景节点类
 */
class IMapLine:public IMapSceneNode
{
public:
    CONSTRUCTOR(IMapLine,IMapSceneNode)

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
    virtual std::vector<MapGeoPos> GetMulPos()const =0;

    /**
     * @brief 设置点的大小
     */
    void SetLineWidth(float fLineWidth){if(fLineWidth>0.f&&fabs(fLineWidth-m_fLineWidth)>FLT_EPSILON){m_fLineWidth=fLineWidth;LineWidthChanged();}}
    const float& GetLineWidth()const {return(m_fLineWidth);}

    /**
     * @brief 设置线型
     */
    void SetLineType(MAP_LINE_TYPE lineType)JUDGE_EQUAL_CALL_FUNCTION(lineType,m_emLineType,LineTypeChanged)
    MAP_LINE_TYPE GetLineType()const {return(m_emLineType);}

    /**
     * @brief 设置点的大小
     */
    void SetLineColor(const SceneColor& lineColor)JUDGE_EQUAL_CALL_FUNCTION(lineColor,m_stLineColor,LineColorChanged)
    const SceneColor& LineColor()const{return(m_stLineColor);}
protected:
    virtual ~IMapLine(){}

    /**
     * @brief 线的状态修改
     */
    virtual void LineWidthChanged()=0;
    virtual void LineTypeChanged()=0;
    virtual void LineColorChanged()=0;

protected:
    std::list<MapGeoPos>   m_listAllPos;
    float                  m_fLineWidth=1.f;
    MAP_LINE_TYPE          m_emLineType=MAP_SOLID_LINE;
    SceneColor             m_stLineColor;
};

#endif // INTERFACE_MAP_LINE_HEARDER_H
