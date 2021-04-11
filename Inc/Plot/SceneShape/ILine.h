#ifndef INTERFACE_LINE_HEARDER_H
#define INTERFACE_LINE_HEARDER_H
#include <vector>
#include <Plot/SceneShape/IGeometry.h>
/**
 * @brief 绘制线
 */
class ILine:public IGeometry
{
public:
    enum LINE_TYPE
    {
        SOLID_LINE, /// 实线
        DOTTED_LINE /// 虚线
    };

    CONSTRUCTOR(ILine,IGeometry)

    /**
     * @brief 设置线宽度
     */
    virtual void SetLineWidth(unsigned int nWidth)JUDGE_EQUAL_CALL_FUNCTION(nWidth,m_nLineWidth,LineWidthChanged)
    virtual unsigned int GetLineWidth(){return(m_nLineWidth);}

    /**
     * @brief 设置线型
     */
    void SetLineType(LINE_TYPE lineType)JUDGE_EQUAL_CALL_FUNCTION(lineType,m_emLineType,LineTypeChanged)
    LINE_TYPE GetLineType()const {return(m_emLineType);}

protected:
    virtual void LineWidthChanged()=0;
    virtual void LineTypeChanged()=0;
    virtual ~ILine(){}
protected:
    unsigned int m_nLineWidth=1u;
    LINE_TYPE    m_emLineType=SOLID_LINE;
};

#endif // INTERFACE_LINE_HEARDER_H
