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

    /**
     * @brief 是否显示背面
     * @param bShow
     */
    void SetShowBack(bool bShow)JUDGE_EQUAL_CALL_FUNCTION(bShow,m_bShowBack,ShowBackChanged)
    bool ShowBack()const{return(m_bShowBack);}

protected:
    virtual ~IShape(){}
    /**
     * @brief 颜色更改消息
     */
    virtual void ColorChanged()=0;
    virtual void ShapeChanged()=0;
    virtual void ShowBackChanged()=0;

protected:
    SceneColor m_stColor;
    bool       m_bShowBack=true;
};

#endif//INTERFACE_SHAPE_HEARDER_H
