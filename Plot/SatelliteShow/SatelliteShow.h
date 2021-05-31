#ifndef CSATELLITESHOW_H
#define CSATELLITESHOW_H

#include <NoQt.h>
/**
 * @brief 显示卫星的类
 */

#include <Plot/SatelliteShow/ISatellite.h>
#include <Plot/SceneShape/ILabel.h>

#include <Inner/Common/ImplSceneGroup.hpp>

class ILine;
class IImage;

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

    /// <summary>
    /// 设置卫星字体
    /// </summary>
    /// <param name="fontSize">字体大小</param>
    /// <param name="fontFillColor">字体颜色</param>
    /// <param name="fontOutColor">字体边框颜色</param>
    void SetFont(int fontSize, SceneColor fontFillColor, SceneColor fontOutColor);

    /**
     * @brief 设置卫星图标路径
     */
    void SetPicPath(const std::string& sPicPath);

    /**
     * @brief 设置分级显示距离
     */
    void SetLodDis(double dis);
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
    ISceneAttitudeGroup*       m_pSatellite{};      //卫星根节点

    ISceneAttitudeGroup*       m_pSatelliteAtt{};     //卫星调姿节点
    ISceneScaleGroup*          m_pSatelliteScale{}; //卫星缩放节点
    ISceneModel*               m_pModel{};    //3D模型
    IImage*                    m_pImage{};  //2D图标
    double                     m_lodDis{1e6};  //分级显示距离
    ILabel*                    m_pSatelliteName{};

    ILine*                     m_pOribit{};
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

    std::string m_sPicPath;     //2D图标路径

    double                     m_dStart{0.};
    double                     m_dEnd{0.};
    double                     m_dStep{0.};
    int                        m_nIndex{-1};
};

extern "C"
{
    Q_DECL_EXPORT ISatellite* CreateNode(ISceneGraph*pSceneGraph,const std::string& sInterfaceName);
    Q_DECL_EXPORT bool QueryInterface(std::string& sInterfaceName);
}
#endif // CSATELLITESHOW_H
