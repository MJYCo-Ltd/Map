#ifndef PLANETMODEL_H
#define PLANETMODEL_H
#include <osg/MatrixTransform>
#include <osgText/Text>

class EllipsoidBounding;
class ISceneGraph;
class PlanetUpdateCallback;
class PlantDrawCallback;

class CPlanetModel:public osg::MatrixTransform
{
    friend class CSolarEnv;
    friend class PlanetUpdateCallback;
    friend class PlantDrawCallback;

public:
    CPlanetModel(ISceneGraph* pSceneGraph);

    /**
     * @brief 设置行星的名字
     * @param pPlanetName
     */
    void SetName(const wchar_t* pPlanetName,const char* pImageName);

    /**
     * @brief 更新行星位置
     * @param rPos
     */
    void UpdatePostion(const osg::Vec3& rPos);
    const osg::Vec3& GetUpdatePostion(){return(m_rECIPostion);}

    /**
     * @brief 更新行星眼的位置
     * @param rEyePos
     */
    void UpdateEyePostion(const osg::Vec3& rEyePos);
    const osg::Vec3& GetEyePostion(){return(m_rEyePostion);}

    /**
     * @brief 设置颜色
     * @param rColor
     */
    void SetColor(const osg::Vec4& rColor);

    /**
     * @brief 设置行星的半径
     * @param radiusEquator
     * @param radiusPolar
     */
    void SetRadius(float fEquatorRadius,float fPolarRadius);

    /**
     * @brief 需要回调
     * @return
     */
    bool& NeedUpdate(){return(m_bNeedUpdate);}

    /**
     * @brief 设置名字是否可见
     * @param bVisble
     */
    void SetNameVisible(bool bVisble);

protected:
    /**
     * @brief 构建行星
     */
    void MakePlanet();
private:
    ISceneGraph* m_pSceneGraph;
    float       m_fEquatorRadius;                   /// 赤道半径
    float       m_fPolarRadius;                     /// 极半径
    bool        m_bNeedUpdate;                      /// 需要更新
    bool        m_bCulled;                          /// 是否被裁剪掉
    osg::Vec3    m_rECIPostion;                     /// 位置
    osg::Vec3    m_rEyePostion;                     /// 在视坐标下的位置
    osg::Vec4    m_rColor;                          /// 颜色
    std::wstring m_wsPlanetName;                    /// 行星显示名称
    std::string  m_sPlanetImage;                    /// 行星图片名称
    osg::ref_ptr<PlanetUpdateCallback> m_pCallBack; /// 绘制回调
    osg::ref_ptr<PlantDrawCallback>    m_pDrawBack; /// 绘制回调
    osg::ref_ptr<EllipsoidBounding>    m_pBounding; /// 包围盒
    osg::ref_ptr<osg::Uniform>         m_pUniform;  /// 设置一个变量
    osg::ref_ptr<osgText::Text>        m_pShowName; /// 行星名称
};

#endif // PLANETMODEL_H
