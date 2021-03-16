#ifndef CMAPRECTANGE_H
#define CMAPRECTANGE_H
#include "ExternMapShape_Global.h"
#include <Plot/Map/IMapPolygon.h>

/**
 * @brief 在地图上绘制矩形
 */
class OSGEXTERNSHARED_EXPORT CMapRectange
{
public:
    CMapRectange(ISceneGraph* pSceneGraph);

    /**
     * @brief 设置左上角
     * @param rLeftUp
     */
    void SetLeftUp(const MapGeoPos& rLeftUp);

    /**
     * @brief 设置右下角
     * @param rRightDown
     * @attention 不会进行和左上角的大小比较
     * @attention 如果右下角的经度小于左上角的经度
     *            那么会支持超过半球的区域
     */
    void SetRightDown(const MapGeoPos& rRightDown);

    /**
     * @brief 获取地图节点
     * @return
     */
    IMapPolygon* GetMapSceneNode(){return(m_pPolygon);}
protected:
    /**
     * @brief 初始化节点
     */
    void InitNode();

    /**
     * @brief 更新节点
     */
    void UpdateNode();
protected:
    MapGeoPos     m_stLeftUp;
    MapGeoPos     m_stRightDown;
    ISceneGraph*  m_pSceneGraph=nullptr;
    IMapPolygon*  m_pPolygon=nullptr;
};

#endif // CMAPRECTANGE_H
