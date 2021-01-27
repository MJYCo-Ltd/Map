#ifndef PLANETMODEL_H
#define PLANETMODEL_H
#include <osg/Camera>
#include <osg/MatrixTransform>
#include <osgEarth/Text>

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
    CPlanetModel(ISceneGraph* pSceneGraph,int nIndex);

    /**
     * @brief 更新行星位置
     * @param rPos
     */
    void UpdatePostion(const osg::Vec3& rPos);
    const osg::Vec3& GetUpdatePostion(){return(m_rECIPostion);}


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
    int          m_nRenderBin;
    ISceneGraph* m_pSceneGraph;
    float       m_fEquatorRadius;                   /// 赤道半径
    float       m_fPolarRadius;                     /// 极半径
    osg::Vec3    m_rECIPostion;                     /// 位置
    osg::Vec4    m_rColor;                          /// 颜色
    std::wstring m_wsPlanetName;                    /// 行星显示名称
    std::string  m_sPlanetImage;                    /// 行星图片名称
    osg::ref_ptr<osg::Camera> m_pTransForm;
    osg::ref_ptr<EllipsoidBounding>    m_pBounding; /// 包围盒
    osg::ref_ptr<osg::Uniform>         m_pUniform;  /// 设置一个变量
    osg::ref_ptr<osgEarth::Text>        m_pShowName; /// 行星名称
};

#endif // PLANETMODEL_H
