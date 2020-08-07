#ifndef INTERFACE_LINE_HEARDER_H
#define INTERFACE_LINE_HEARDER_H
#include <vector>
#include "IDrawShape.h"
using namespace std;

enum SCENE_LINE_TYPE
{
    SOLID_LINE, /// 实线
    DOTTED_LINE /// 虚线
};

/**
 * @brief 场景节点类
 */
class ILine:public IDrawShape
{
public:

    /**
     * @brief 增加点
     */
    virtual void AddPoint(const ScenePos&)=0;

    /**
     * @brief 移除点
     */
    virtual void RemovePoint(int)=0;

    /**
     * @brief 更新指定位置点
     */
    virtual void UpdatePoint(int,const ScenePos&)=0;

    /**
     * @brief 设置多个位置
     */
    virtual void SetMultPos(const vector<ScenePos>&)=0;
    virtual const vector<ScenePos>& GetMulPos()=0;
    /**
     * @brief 设置点的大小
     */
    virtual void SetLineWidth(float)=0;
    virtual const float& GetLineWidth()=0;

    /**
     * @brief 设置线型
     */
    virtual void SetLineType(SCENE_LINE_TYPE)=0;
    virtual SCENE_LINE_TYPE GetLineType()=0;

    /**
     * @brief 设置点的大小
     */
    virtual void SetColor(const SceneColor&)=0;
    virtual const SceneColor& GetColor()=0;
};

#endif
