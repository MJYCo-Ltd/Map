#ifndef CMAP_INCLUDE_HEADER_H
#define CMAP_INCLUDE_HEADER_H
#include <map>
#include <osgEarth/MapNode>
#include <osgEarth/Lighting>
#include <Map/IMap.h>
#include <Inner/Common/ImplSceneGroup.hpp>
#include <SceneGraph/IWindow.h>
#include <Inner/OsgExtern/MapNodeCullBack.h>

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
    bool ConvertCoord(float& fX, float& fY, ScenePos &geoPos, short TranType) override;

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
     * @brief时间更新
     */
    void DateChanged() override SET_TRUE_NODE_UPDATE(m_bDateChanged)

    /**
     * @brief 鼠标移动消息
     */
    void MouseMove(MouseButtonMask, int, int);
protected:
    /**
     * @brief 初始化节点
     */
    void InitNode() override;

    /**
     * @brief 更新节点
     */
    void UpdateNode() override;

    /**
     * @brief 初始化地图
     */
    void InitMap();

    /**
     * @brief 初始化3D灯光
     */
    void Init3DLight();
protected:
    bool   m_bSelfRotate=true;
    bool   m_bDateChanged=false;
    std::list<IMapMessageObserver*> m_listObserver;
    osg::ref_ptr<osg::Group>   m_p2DRoot;
    osg::ref_ptr<osg::Group>   m_p3DRoot;

    osg::ref_ptr<osgEarth::MapNode> m_pMap3DNode;
    osg::ref_ptr<osgEarth::MapNode> m_pMap2DNode;

    osg::ref_ptr<osgEarth::LightGL3> m_pLight;
    osg::ref_ptr<osg::Uniform>       m_pLightPosUniform;
    osg::ref_ptr<CMapNodeCullBack>   m_pUpdateCallBack;

    UserLayers   m_userLayers;
    MapLayers    m_earthFileLayers;
    MapType      m_emType=MAP_3D;
    CSpaceEnv   *m_pSpaceEnv=nullptr;/// 空间背景
    CAtmosphere *m_pAtmosphere=nullptr;///大气效果
};

#include <NoQt.h>
extern "C"
{
    Q_DECL_EXPORT ISceneNode* CreateNode(ISceneGraph*pSceneGraph,const std::string& sInterfaceName);
    Q_DECL_EXPORT bool QueryInterface(std::string& sInterfaceName);
}
#endif // CEARTH_H
