#ifndef CMAPRECTANGE_H
#define CMAPRECTANGE_H
#include <ExternShape/ExternShape_Global.h>
#include <Plot/Map/IMapPolygon.h>

/**
 * @brief 在地图上绘制矩形
 */
class OSGEXTERNSHARED_EXPORT CMapRectange
{
public:
    CMapRectange(ISceneGraph* pSceneGraph);

    /**
     * @brief 设置点1
     * @param rLeftUp
     */
    void SetPos1(const ScenePos& pos);

    /**
     * @brief 设置点2
     * 根据点1、点2的位置关系，自动计算其余两点位置
     */
    void SetPos2(const ScenePos &pos);
    /**
     * @brief 设置颜色
     */
    void SetColor(const SceneColor& color);
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
    ScenePos     m_stPos1;
    ScenePos     m_stPos2;
    ISceneGraph*  m_pSceneGraph=nullptr;
    IMapPolygon*  m_pPolygon=nullptr;
};

#endif // CMAPRECTANGE_H
