#ifndef CMAP_INCLUDE_HEADER_H
#define CMAP_INCLUDE_HEADER_H

#include <osg/Camera>
#include <osgEarth/MapNode>
#include <Map/IMap.h>
#include <Inner/QtOsgSceneNode.h>
#include "Map_Global.h"

class CPlotManager;

class CMap:public QtOsgSceneNode<IMap>
{
public:
    CMap(MapType type,ISceneGraph* pSceneGraph);
    ~CMap();

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
    bool ConvertCoord(int& nX, int& nY, ScenePos& geoPos, short TranType);

    /**
     * @brief 获取地图图层
     * @return
     */
    virtual const MapLayers& GetMapLayers();

    /**
     * @brief 控制地图图层显隐
     * @param
     */
    virtual void SetLayerVisible(const string& sLayerName);

    /**
     * @brief 更改地图类型
     */
    virtual void ChangeMapType(MapType mapType);

    /**
     * @brief 获取标绘管理类
     * @return
     */
    IPlotManager* GetPlotManager();

    /**
     * @brief 获取星空背景
     * @return
     */
    ISpaceEnv* GetSpaceEnv();

protected:
    void InitSceneNode();

    /**
     * @brief 初始化地图
     */
    void InitMap();

    void LoadSpaceEnv();
protected:
    list<IMapMessageObserver*> m_listObserver;
    osg::ref_ptr<osg::Camera>  m_pCamera;
    osg::ref_ptr<osg::Group>   m_p2DRoot;

    osg::ref_ptr<osgEarth::MapNode> m_pMap3DNode;
    osg::ref_ptr<osgEarth::MapNode> m_pMap2DNode;
    MapLayers    m_allLayers;
    MapType      m_emType;
    CPlotManager* m_pPlotManager=nullptr;
    ISpaceEnv*    m_pSpaceEnv=nullptr;
};

extern "C"
{
    MAPSHARED_EXPORT IMap* CreateMap(MapType, ISceneGraph*);
}
#endif // CEARTH_H
