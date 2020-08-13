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
     * @brief 设置卫星J2000下的位置和速度
     * @param vTime        各个计算点对应的 约简儒略日 时间
     * @param rOribitInfo  卫星的 J2000下的位置和速度   [m,m,m,m/s,m/s,m/s]
     */
    virtual void SetJ2000Oribit(const vector<double> &vTime, const vector<Math::CVector> &rOribitInfo)=0;

    /**
     * @brief 设置卫星的经纬度
     * @param vOribitInfo 卫星的 经纬信息 [rag,rag]
     */
    virtual void SetGeoOribit(const vector<Math::CVector>& vOribitInfo)=0;

};

#endif
