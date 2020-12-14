#ifndef INTERFACE_LINE_HEARDER_H
#define INTERFACE_LINE_HEARDER_H
#include <vector>
#include <Plot/SceneShape/IShape.h>
/**
 * @brief 场景节点类
 */
class ILine:public IShape
{
public:
    CONSTRUCTOR(ILine,IShape)

    /**
     * @brief 增加点
     */
    virtual void AddPoint(int,const ScenePos&)=0;

    /**
     * @brief 移除点
     */
    virtual bool RemovePoint(int)=0;

    /**
     * @brief 更新指定位置点
     */
    virtual bool UpdatePoint(int,const ScenePos&)=0;

    /**
     * @brief 设置多个位置
     */
    virtual void SetMultPos(const std::vector<ScenePos>&)=0;
    virtual std::vector<ScenePos> GetMulPos()const=0;

    /**
     * @brief 设置线宽度
     */
    virtual void SetLineWidth(unsigned int nWidth)JUDGE_EQUAL_CALL_FUNCTION(nWidth,m_nLineWidth,LineWidthChanged)
    virtual unsigned int GetLineWidth(){return(m_nLineWidth);}

protected:
    virtual void LineWidthChanged(){}
    virtual ~ILine(){}
protected:
    unsigned int m_nLineWidth=1u;
};

#endif // INTERFACE_LINE_HEARDER_H
