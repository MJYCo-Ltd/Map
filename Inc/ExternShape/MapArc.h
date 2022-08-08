#ifndef MAPARC_H
#define MAPARC_H

#include <ExternShape/ExternShape_Global.h>
#include <Plot/Map/IMapLine.h>

class OSGEXTERNSHARED_EXPORT CMapArc
{
public:
    CMapArc(ISceneGraph* pSceneGraph);

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
     * @brief 设置开始和结束角度
     */
    void SetAngle(double dStartAngle, double dEndAngle);
    /**
     * @brief 获取地图节点
     * @return
     */
    IMapLine* GetMapSceneNode(){return(m_pMapLine);}
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
    ISceneGraph*  m_pSceneGraph{};
    IMapLine*     m_pMapLine{};

    float         m_fRadius{};
    double        m_dStartAngle;
    double        m_dEndAngle;
    int           m_nFineness{72};
};

#endif // MAPARC_H
