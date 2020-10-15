#ifndef INTERFACE_COVER_HEARDER_H
#define INTERFACE_COVER_HEARDER_H
#include <vector>
#include <string>
#include "IDrawShape.h"
using namespace std;

/**
 * @brief 位置信息
 */
struct PosValue
{
    double dX;
    double dY;
    double dValue;
};

/**
 * @brief 覆盖信息
 */
struct CoverInfo
{
    int    nXNum;
    int    nYNum;
    double dMin;
    double dMax;
    vector<PosValue>vPosInfo;
};

/**
 * @brief 测点信息
 */
struct CeDian
{
    double dLon;
    double dLat;
    string sName;
};

/**
 * @brief 绘制地面覆盖类
 */
class ICover:public IDrawShape
{
public:

    /**
     * @brief 获取覆盖范围
     * @return
     */
    virtual void SetCoverInfo(const CoverInfo&)=0;

    /**
     * @brief 设置测点信息
     */
    virtual void SetCeDian(const vector<CeDian>&)=0;

    /**
     * @brief 设置最小值的颜色
     */
    virtual void SetMinValueColor(const SceneColor&)=0;

    /**
     * @brief 设置最大值的颜色
     */
    virtual void SetMaxValueColor(const SceneColor&)=0;

    /**
     * @brief 设置范围和图片
     */
    virtual void SetBound(const ScenePos&,const ScenePos&,const string&)=0;
};

#endif
