#ifndef INTERFACE_ORIBIT_HEARDER_H
#define INTERFACE_ORIBIT_HEARDER_H


#include <vector>
#include <Math/Vector.h>
#include "../IMapSceneNode.h"
using namespace std;

class ISensor;

/**
 * @brief 场景节点类
 */
class IOribit:public ISceneNode
{
public:

    /**
     * @brief 设置轨道颜色
     */
    virtual void SetOribitColor(const SceneColor&)=0;

    /**
     * @brief 设置卫星J2000下的位置和速度
     * @param 第二个参数 卫星的 J2000下的位置和速度   [m,m,m,m/s,m/s,m/s]
     */
    virtual void SetJ2000Oribit(const vector<Math::CVector> &)=0;

    /**
     * @brief 设置卫星的经纬度
     * @param 经纬信息 [deg,deg]
     */
    virtual void SetGeoOribit(const vector<Math::CVector>&)=0;

};

#endif
