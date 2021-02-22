#ifndef CSATELLITESHOW_H
#define CSATELLITESHOW_H

#include <NoQt.h>
/**
 * @brief 显示卫星的类
 */

#include <Plot/SatelliteShow/ISatellite.h>
#include <Plot/SceneShape/ILabel.h>

#include <Inner/ImplSceneGroup.hpp>

class ILine;

class CSatelliteShow:public ImplSceneGroup<ISatellite>
{
public:
    CONSTRUCTOR(CSatelliteShow,ImplSceneGroup<ISatellite>)

    /**
     * @brief 初始化节点
     */
    void InitNode();

    /**
     * @brief 设置轨道数据
     */
    void SetJ2000Oribit(const std::vector<double> &, const std::vector<Math::CVector> &);

    /**
     * @brief 更新轨道数据
     */
    void SetECFOribit(const std::vector<Math::CVector>&);

    /**
     * @brief 添加传感器
     * @param pSensor
     */
    void AddSensor(int id, ISensor*pSensor);

    /// <summary>
    /// 获取卫星WGS84位置和速度
    /// </summary>
    /// <returns></returns>
    const Math::CVector& GetSatelliteWgs84PV()const;


    /// <summary>
    /// 设置卫星缩放
    /// </summary>
    /// <param name="dScale"></param>
    void SetScale(double dScale);

    /// <summary>
    /// 设置卫星3D模型的矫正姿态
    /// </summary>
    /// <param name="rAttitude"></param>
    void SetCorrectAttitude(const SceneAttitude& rAttitude);

    /// <summary>
    /// 设置卫星姿态
    /// </summary>
    /// <param name="rAttitude"></param>
    void SetAttitude(const SceneAttitude& rAttitude);

    /// <summary>
    /// 获取卫星姿态
    /// </summary>
    /// <param name="rAttitude"></param>
    const SceneAttitude& GetAttitude()const;

    /// <summary>
    /// 设置卫星传感器姿态
    /// </summary>
    /// <param name="rAttitude"></param>
    void SetSensorAttitude(int id, const SceneAttitude& rAttitude);

    /// <summary>
    /// 获取卫星传感器姿态
    /// </summary>
    /// <param name="rAttitude"></param>
    const SceneAttitude& GetSensorAttitude(int id)const;

    /// <summary>
    /// 按给定的时长更新轨道的显示
    /// </summary>
    /// <param name="duration">时长</param>
    void UpdateJ2000OribitShow(double duration);
protected:
    void ModelChanged();
    void NameChanged();
    void NowTimeChanged();
    void OribitColorChanged();
    /**
     * @brief 根据时间计算卫星轨道数据
     */
    double CalItNewton(double *dX, double dT, int nDim);
    double CalItNewtonEcf(double*,double,int);

protected:
    ISceneAttitudeGroup*       m_pSatellite = nullptr;      //卫星根节点

    ISceneAttitudeGroup*       m_pSatelliteAtt=nullptr;     //卫星调姿节点
    ISceneScaleGroup*          m_pSatelliteScale = nullptr; //卫星缩放节点
    ISceneModel*               m_pModel=nullptr;
    ILabel*                    m_pSatelliteName=nullptr;

    ILine*                     m_pOribit=nullptr;
    std::vector<Math::CVector> m_vOribit;    /// J2000坐标系下的数据
    std::vector<Math::CVector> m_vEcfOribit; /// 地固系下的轨道数据

    std::set<ISensor*>              m_vSensor;
    std::map<int, ISceneAttitudeGroup*> m_SensorAttMap; //传感器调姿map
    std::map<int, SceneAttitude> m_SensorAttValueMap;   //传感器姿态map

    std::vector<double>        m_vdMjd;      /// 坐标对应的时间
    Math::CVector              m_stNowPos; /// 当前卫星的位置(j2000)
    Math::CVector              m_satelliteWgs84PV;  // 当前卫星的位置和速度(wgs84)
    SceneAttitude              m_satelliteCorrectAttitude;  //卫星模型矫正姿态
    SceneAttitude              m_satelliteAttitude;         //卫星姿态


    double                     m_dStart = 0.0;
    double                     m_dEnd = 0.0;
    double                     m_dStep = 0.0;
    int                        m_nIndex = -1;
};

extern "C"
{
    Q_DECL_EXPORT ISatellite* CreateNode(ISceneGraph*pSceneGraph,const std::string& sInterfaceName);
    Q_DECL_EXPORT bool QueryInterface(std::string& sInterfaceName);
}
#endif // CSATELLITESHOW_H
