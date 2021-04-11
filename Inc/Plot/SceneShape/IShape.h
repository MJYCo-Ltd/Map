#ifndef INTERFACE_SHAPE_HEARDER_H
#define INTERFACE_SHAPE_HEARDER_H
#include <SceneGraph/ISceneNode.h>

/**
 * @brief 绘制模型的形状节点
 */
class IShape:public ISceneNode
{
public:
    CONSTRUCTOR(IShape,ISceneNode)

    /**
     * @brief 设置颜色
     */
    void SetColor(const SceneColor& rColor)JUDGE_EQUAL_CALL_FUNCTION(rColor,m_stColor,ColorChanged)
    const SceneColor& Color() const{return(m_stColor);}

protected:
    virtual ~IShape(){}
    /**
     * @brief 颜色更改消息
     */
    virtual void ColorChanged()=0;
    virtual void ShapeChanged()=0;

protected:
    SceneColor m_stColor;
};

#endif//INTERFACE_SHAPE_HEARDER_H
