#ifndef INTERFACE_POLYGON_HEARDER_H
#define INTERFACE_POLYGON_HEARDER_H

#include <Plot/SceneShape/IGeometry.h>

/**
 * @brief 多边形节点
 * @attention 多边形的边不能自相交
 */
class IPolygon:public IGeometry
{
public:
    CONSTRUCTOR(IPolygon,IGeometry)

    /**
     * @brief 是否显示边界线
     */
    void ShowOutLine(bool bShowOutLine)JUDGE_EQUAL_CALL_FUNCTION(bShowOutLine,m_bShowOutLine,DrawChanged);
    bool IsShowOutLine();

    /**
     * @brief 是否填充
     */
    void Fill(bool bFille)JUDGE_EQUAL_CALL_FUNCTION(bFille,m_bFill,DrawChanged)
    bool IsFill(){return(m_bFill);}
protected:
    virtual ~IPolygon(){}

    virtual void DrawChanged()=0;
protected:
    bool m_bShowOutLine{false};
    bool m_bFill{true};
};

#endif//INTERFACE_POLYGON_HEARDER_H
