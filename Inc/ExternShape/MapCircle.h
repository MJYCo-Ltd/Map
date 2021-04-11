#ifndef CMAPCIRCLE_H
#define CMAPCIRCLE_H
#include <ExternShape/ExternShape_Global.h>
#include <Plot/Map/IMapPolygon.h>

/**
 * @brief 在地图上绘制矩形
 */
class OSGEXTERNSHARED_EXPORT CMapCircle
{
public:
    CMapCircle(ISceneGraph* pSceneGraph);

    /**
     * @brief 设置圆心
     * @param rLeftUp
     */
    void SetCenter(const ScenePos& pos);
	
    /**
     * @brief 设置半径
     */
    void SetRadius(const float&);
    /**
     * @brief 设置颜色
     */
    void SetColor(const SceneColor& color);

    /**
     * @brief 设置细度
     */
    void SetFineness(int);
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
    ScenePos     m_stCenter;
    ISceneGraph*  m_pSceneGraph=nullptr;
    IMapPolygon*  m_pPolygon=nullptr;

    float         m_fRadius = 0.0f;
    int           m_nFineness = 36;
};

#endif // CMAPCIRCLE_H
