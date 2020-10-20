#ifndef INTERFACE_SATELLITE_HEARDER_H
#define INTERFACE_SATELLITE_HEARDER_H
#include <string>
#include <vector>
#include <Math/Vector.h>
#include "../IMapSceneNode.h"
using namespace std;

class ISensor;

/**
 * @brief 场景节点类
 */
class ISatellite:public IMapSceneNode
{
public:
    /**
     * @brief 设置名称
     */
    virtual void SetName(const string&)=0;

    /**
     * @brief 设置卫星模型路径
     */
    virtual void SetModelPath(const string&)=0;

    /**
     * @brief 设置轨道颜色
     */
    virtual void SetOribitColor(const SceneColor&)=0;

    /**
     * @brief 设置卫星J2000下的位置和速度
     * @param 第一个参数 各个计算点对应的 约简儒略日 时间
     * @param 第二个参数 卫星的 J2000下的位置和速度   [m,m,m,m/s,m/s,m/s]
     */
    virtual void SetJ2000Oribit(const vector<double> &, const vector<Math::CVector> &)=0;

    /**
     * @brief 设置卫星的地固系的位置
     */
    virtual void SetECFOribit(const vector<Math::CVector>&)=0;

    /**
     * @brief 更新时间
     */
    virtual void UpdateData(double)=0;

};

#endif
