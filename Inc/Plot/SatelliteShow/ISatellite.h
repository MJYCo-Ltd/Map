#ifndef INTERFACE_SATELLITE_HEARDER_H
#define INTERFACE_SATELLITE_HEARDER_H
#include <string>
#include <vector>
#include <Math/Vector.h>

#include <SceneGraph/ISceneGroup.h>

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
    void SetName(const std::string& sName)JUDGE_EQUAL_CALL_FUNCTION(sName,m_sName,NameChanged)

    /**
     * @brief 设置卫星模型路径
     */
    void SetModelPath(const std::string& sModelPath)JUDGE_EQUAL_CALL_FUNCTION(sModelPath,m_sModelPath,ModelChanged)

    /**
     * @brief 设置轨道颜色
     */
    void SetOribitColor(const SceneColor& rOribitColor)JUDGE_EQUAL_CALL_FUNCTION(rOribitColor,m_sColor,OribitColorChanged)

    /**
     * @brief 设置卫星J2000下的位置和速度
     * @param 第一个参数 各个计算点对应的 约简儒略日 时间
     * @param 第二个参数 卫星的 J2000下的位置和速度   [m,m,m,m/s,m/s,m/s]
     */
    virtual void SetJ2000Oribit(const std::vector<double> &, const std::vector<Math::CVector> &)=0;

    /**
     * @brief 设置卫星的地固系的位置
     */
    virtual void SetECFOribit(const std::vector<Math::CVector>&) = 0;

    /// <summary>
    /// 获取卫星WGS84位置和速度
    /// </summary>
    /// <returns></returns>
    virtual const Math::CVector& GetSatelliteWgs84PV()const = 0;

    /// <summary>
    /// 设置卫星缩放
    /// </summary>
    /// <param name="dScale"></param>
    virtual void SetScale(double dScale) = 0;

    /// <summary>
    /// 设置卫星3D模型的矫正姿态
    /// </summary>
    /// <param name="rAttitude"></param>
    virtual void SetCorrectAttitude(const SceneAttitude& rAttitude) = 0;

    /// <summary>
    /// 设置卫星姿态
    /// </summary>
    /// <param name="rAttitude"></param>
    virtual void SetAttitude(const SceneAttitude& rAttitude) = 0;

    /// <summary>
    /// 获取卫星姿态
    /// </summary>
    /// <param name="rAttitude"></param>
    virtual const SceneAttitude& GetAttitude()const = 0;

    /// <summary>
    /// 设置卫星传感器姿态
    /// </summary>
    /// <param name="rAttitude"></param>
    virtual void SetSensorAttitude(int id, const SceneAttitude& rAttitude) = 0;

    /// <summary>
    /// 获取卫星传感器姿态
    /// </summary>
    /// <param name="rAttitude"></param>
    virtual const SceneAttitude& GetSensorAttitude(int id) const = 0;

    /// <summary>
    /// 按给定的时长更新轨道的显示
    /// </summary>
    /// <param name="beginTime">开始时间（s）</param>
    /// <param name="duration">时长(s)</param>
    virtual void UpdateJ2000OribitShow(double beginTime, double duration) = 0;

    /**
     * @brief 更新时间
     */
    void UpdateData(double dMJD)JUDGE_DOUBLE_CALL_FUNCTION(dMJD, m_dNowMJD, NowTimeChanged)

    /**
     * @brief添加传感器
     */
    virtual void AddSensor(int id, ISensor*) = 0;

    /// <summary>
    /// 设置卫星字体
    /// </summary>
    /// <param name="fontSize">字体大小</param>
    /// <param name="fontFillColor">字体颜色</param>
    /// <param name="fontOutColor">字体边框颜色</param>
    virtual void SetFont(int fontSize, SceneColor fontFillColor, SceneColor fontOutColor) = 0;

    /**
     * @brief 设置卫星图标路径
     */
    virtual void SetPicPath(const std::string& sPicPath) = 0;

    /**
     * @brief 设置分级显示距离
     */
    virtual void SetLodDis(double dis) = 0;

    /**
     * @brief 清理2D节点
     */
    virtual void Clear2DNodes() = 0;

    /**
     * @brief 设置轨道线宽
     */
    virtual void SetOribitWidth(int width) = 0;
protected:
    virtual ~ISatellite(){}
    virtual void ModelChanged()=0;
    virtual void NameChanged()=0;
    virtual void NowTimeChanged()=0;
    virtual void OribitColorChanged()=0;
protected:
    std::string m_sName;
    std::string m_sModelPath;
    SceneColor m_sColor;
    double m_dNowMJD{};
};

#endif
