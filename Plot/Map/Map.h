#ifndef CMAP_INCLUDE_HEADER_H
#define CMAP_INCLUDE_HEADER_H
#include <map>
#include <osg/Depth>
#include <osgEarth/MapNode>
#include <osgEarth/Lighting>
#include <Plot/Map/IMap.h>
#include <Inner/Common/ImplSceneGroup.hpp>
#include <Inner/ThreadSafety/SafetySet.h>
#include <SceneGraph/IWindow.h>

class CSpaceEnv;
class ISceneGraph;
class CAtmosphere;

class CMapLayer;
typedef std::map<std::string,CMapLayer*> UserLayers;


class CMap:public ImplSceneGroup<IMap>,public IWindowMessageObserver
{
    friend class CMapNodeChanged;
public:
    CONSTRUCTOR(CMap,ImplSceneGroup<IMap>)
    ~CMap();

    /**
     * @brief 初始化地图
     * @param 地图路径
     */
    bool LoadUserMap(const std::string& sFileName,bool bRef) override;

    /**
     * @brief 设置地图类型
     * @param type
     */
    void SetType(MapType type){m_emType = type;}

    /**
     * @brief 注册消息
     */
    void SubMessage(IMapMessageObserver* pMsgObr) override;

    /**
     * @brief 取消注册消息
     */
    void UnSubMessage(IMapMessageObserver* pMsgObr) override;

    /**
     * @brief 坐标转换
     * @param nX        屏幕坐标X
     * @param nY        屏幕坐标X
     * @param geoPos    地理坐标
     * @param TranType  转换类型 0表示从屏幕坐标转换成地理坐标，1表示由地理坐标转换成屏幕坐标
     * @return
     */
    bool ConvertCoord(int& fX, int& fY, ScenePos &geoPos, short TranType) override;

    /**
     * @brief 获取指定位置的高程
     */
    float GetHeight(float fLon, float fLat)override;

    /**
     * @brief 获取地图图层
     * @return
     */
    virtual MapLayers GetMapLayers() const override;

    /**
     * @brief 控制地图图层显隐
     * @param
     */
    virtual void SetLayerVisible(const std::string& sLayerName) override;

    /**
     * @brief 创建图层
     * @return
     */
    IMapLayer* CreateLayer(const std::string&) override;
    virtual bool RemoveLayer(IMapLayer*&) override;
    virtual bool RemoveLayer(const std::string& sLayerName) override;
    /**
     * @brief 清空所有图层
     */
    void ClearLayers() override;

    /**
     * @brief 更改地图类型
     */
    virtual void ChangeMapType(MapType mapType) override;
    MapType GetMapType()const override{return(m_emType);}

    /**
     * @brief 获取地惯系节点
     * @return
     */
    virtual ISpaceEnv* GetSpaceEnv() override;

    /**
     * @brief 设置地球是否自转
     */
    void SetEarthSelfRotate(bool) override;

    /**
     * @brief 设置显示大气效果
     */
    void SetShowAtmosphere(bool)override;

    /**
     * @brief 设置黑夜的颜色
     */
    void SetNightColor(const SceneColor& rColor) override;

    /**
     * @brief时间更新
     */
    void DateChanged() override {m_bDateChanged=true;}

    /**
     * @brief 鼠标移动消息
     */
    void MouseMove(MouseButtonMask, int, int)override;
    /**
     * @brief 鼠标点击消息
     */
    void MouseDown(MouseButtonMask, int, int)override;
protected:
    /**
     * @brief 初始化节点
     */
    void InitNode() override;

    /**
     * @brief 更新节点
     */
    void FrameCall() override;

    /**
     * @brief 加载地图
     */
    void LoadMap();

    /**
     * @brief 初始化3D灯光
     */
    void Init3DLight();

    /**
     * @brief 从指定位置删除图层
     */
    void RemoveLayer(UserLayers::iterator itor);

protected:
    int    m_nX{};
    int    m_nY{};

    bool   m_bSelfRotate{false};
    bool   m_bDateChanged{false};
    bool   m_bMapChanged{false};
    bool   m_bInstelld{false};
    bool   m_bMouseMove{false};
    bool   m_bIs3D{false};

    ScenePos m_stMousePos;                      ///保留鼠标位置
    SceneColor m_stNightColor;
    osgEarth::GeoExtent m_extent;
    std::string m_sUserMapPath;
    CSafetySet<IMapMessageObserver*> m_setObserver;
    osg::ref_ptr<osg::Group>   m_p3DRoot;
    osg::ref_ptr<osg::Group>   m_pNoTran2DMapNode;
    osg::ref_ptr<osg::MatrixTransform> m_pLeftTran;
    osg::ref_ptr<osg::MatrixTransform> m_pRightTran;

    osg::ref_ptr<osgEarth::MapNode> m_pPreMapNode;
    osg::ref_ptr<osgEarth::MapNode> m_pCurMapNode;

    osg::ref_ptr<osgEarth::LightGL3> m_pLight;
    osg::ref_ptr<osg::Uniform>       m_pLightPosUniform;
    osg::ref_ptr<osg::Depth>         m_pDepth;

    /// 主视口位置
    osg::observer_ptr<osgViewer::View>            m_pView;
    osg::ref_ptr<osgUtil::LineSegmentIntersector> m_pPicker;

    UserLayers   m_userLayers;
    MapLayers    m_earthFileLayers;
    MapType      m_emType{MAP_3D};
    CSpaceEnv   *m_pSpaceEnv{};/// 空间背景
    CAtmosphere *m_pAtmosphere{};///大气效果
};

#include <NoQt.h>
extern "C"
{
    Q_DECL_EXPORT ISceneNode* CreateNode(ISceneGraph*pSceneGraph,const std::string& sInterfaceName);
    Q_DECL_EXPORT bool QueryInterface(std::string& sInterfaceName);
}
#endif // CEARTH_H
