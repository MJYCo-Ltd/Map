#ifndef CSCENELINE_H
#define CSCENELINE_H
#include <list>
#include <osgEarth/LineDrawable>
#include <Plot/SceneShape/ILine.h>
#include <Inner/ImplSceneShape.hpp>
class CSceneLine:public ImplSceneNode<ILine>
{
public:
    CONSTRUCTOR(CSceneLine,ImplSceneNode<ILine>)

    /**
     * @brief增加一个点
     */
    void AddPoint(int,const ScenePos&);

    /**
     * @brief 移除一个点
     * @return
     */
    bool RemovePoint(int);

    /**
     * @brief更新点
     * @return
     */
    bool UpdatePoint(int,const ScenePos&);

    /**
     * @brief 一次设置多个点
     */
    void SetMultPos(const std::vector<ScenePos>&);
    std::vector<ScenePos> GetMulPos()const;

protected:

    /**
     * @brief 线宽更改
     */
    void LineWidthChanged()SET_TRUE_NODE_UPDATE(m_bWidthChanged)

    /**
     * @brief 形状更改
     */
    void ShapeChanged()SET_TRUE_NODE_UPDATE(m_bShapeChanged)

    /**
     * @brief 颜色修改
     */
    void ColorChanged()SET_TRUE_NODE_UPDATE(m_bColorChanged)

    /**
     * @brief 初始化节点
     */
    void InitNode();

    /**
     * @brief 节点更新
     */
    void UpdateNode();
protected:
    osg::ref_ptr<osgEarth::LineDrawable> m_pLine;
    std::list<ScenePos>           m_listAllPos;
    bool                          m_bColorChanged=false;
    bool                          m_bShapeChanged=false;
    bool                          m_bWidthChanged=false;
};

#endif // CSCENELINE_H
