#ifndef CMAP_INCLUDE_HEADER_H
#define CMAP_INCLUDE_HEADER_H
#include <map>
#include <osg/Camera>
#include <osgEarth/MapNode>
#include <Map/IMap.h>
#include <Inner/ImplSceneGroup.hpp>
#include <NoQt.h>

class CSpaceEnv;
class ISceneGraph;

class CMapLayer;
typedef std::map<std::string,CMapLayer*> UserLayers;

class CMap:public ImplSceneGroup<IMap>
{
    friend class CMapNodeChanged;
public:
    CONSTRUCTOR(CMap,ImplSceneGroup<IMap>)
    ~CMap();

    void SetType(MapType type){m_emType = type;}
    /**
     * @brief 注册消息
     */
    void SubMessage(IMapMessageObserver* pMsgObr);

    /**
     * @brief 取消注册消息
     */
    void UnSubMessage(IMapMessageObserver* pMsgObr);

    /**
     * @brief 坐标转换
     * @param nX        屏幕坐标X
     * @param nY        屏幕坐标X
     * @param geoPos    地理坐标
     * @param TranType  转换类型 0表示从屏幕坐标转换成地理坐标，1表示由地理坐标转换成屏幕坐标
     * @return
     */
    bool ConvertCoord(int& nX, int& nY, MapGeoPos &geoPos, short TranType);

    /**
     * @brief 获取地图图层
     * @return
     */
    virtual MapLayers GetMapLayers() const;

    /**
     * @brief 控制地图图层显隐
     * @param
     */
    virtual void SetLayerVisible(const std::string& sLayerName);

    /**
     * @brief 创建图层
     * @return
     */
    IMapLayer* CreateLayer(const std::string&);
    bool RemoveLayer(IMapLayer*&);

    /**
     * @brief 清空所有图层
     */
    void ClearLayers();

    /**
     * @brief 更改地图类型
     */
    virtual void ChangeMapType(MapType mapType);

    /**
     * @brief 获取地惯系节点
     * @return
     */
    virtual ISpaceEnv* GetSpaceEnv();

    /**
     * @brief 设置地球是否自转
     */
    void SetEarthSelfRotate(bool);

protected:
    void InitNode();

    /**
     * @brief 初始化地图
     */
    void InitMap();
protected:
    bool   m_bSelfRotate=true;
    std::list<IMapMessageObserver*> m_listObserver;
    osg::ref_ptr<osg::Group>   m_p2DRoot;

    osg::ref_ptr<osgEarth::MapNode> m_pMap3DNode;
    osg::ref_ptr<osgEarth::MapNode> m_pMap2DNode;

    UserLayers   m_userLayers;
    MapLayers    m_earthFileLayers;
    MapType      m_emType=MAP_3D;
    CSpaceEnv   *m_pSpaceEnv=nullptr;/// 空间背景
};

extern "C"
{
    Q_DECL_EXPORT ISceneNode* CreateNode(ISceneGraph*pSceneGraph,const std::string& sInterfaceName);
    Q_DECL_EXPORT bool QueryInterface(std::string& sInterfaceName);
}
#endif // CEARTH_H
