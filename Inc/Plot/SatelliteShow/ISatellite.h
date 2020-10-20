#ifndef INTERFACE_SATELLITE_HEARDER_H
#define INTERFACE_SATELLITE_HEARDER_H
#include <vector>
#include <Math/Vector.h>

#include <SceneGraph/ISceneGroup.h>
using namespace std;

class ISensor;

/**
 * @brief 场景节点类
 */
class ISatellite:public ISceneGroup
{
public:
    CONSTRUCTOR(ISatellite,ISceneGroup)

    /**
     * @brief 设置名称
     */
    void SetName(const string& sName)JUDGE_EQUAL_CALL_FUNCTION(sName,m_sName,NameChanged)

    /**
     * @brief 设置卫星模型路径
     */
    void SetModelPath(const string& sModelPath)JUDGE_EQUAL_CALL_FUNCTION(sModelPath,m_sModelPath,ModelChanged)

    /**
     * @brief 设置轨道颜色
     */
    void SetOribitColor(const SceneColor& rOribitColor)JUDGE_EQUAL_CALL_FUNCTION(rOribitColor,m_sColor,OribitColorChanged)

    /**
     * @brief 设置卫星J2000下的位置和速度
     * @param 第一个参数 各个计算点对应的 约简儒略日 时间
     * @param 第二个参数 卫星的 J2000下的位置和速度   [m,m,m,m/s,m/s,m/s]
     */
     virtual void SetJ2000Oribit(const vector<double> &, const vector<Math::CVector> &)=0;

    /**
     * @brief 更新时间
     */
    void UpdateData(double dMJD)JUDGE_DOUBLE_CALL_FUNCTION(dMJD,m_dNowMJD,NowTimeChanged)

    /**
     * @brief添加传感器
     */
    virtual void AddSensor(ISensor*)=0;
protected:
    virtual ~ISatellite(){}
    virtual void ModelChanged()=0;
    virtual void NameChanged()=0;
    virtual void NowTimeChanged()=0;
    virtual void OribitColorChanged()=0;
protected:
    string m_sName;
    string m_sModelPath;
    SceneColor m_sColor;
    double m_dNowMJD=0.;
};

#endif
