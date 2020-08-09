#ifndef CPLOTMAP_H
#define CPLOTMAP_H

class ISceneGraph;
class CPlotMap
{
public:
    CPlotMap();

    /**
     * @brief 设置场景图
     * @param pSceneGraph
     */
    void SetSceneGraph(ISceneGraph* pSceneGraph);

    /**
     * @brief 绘制线
     */
    void PlotLine();

    /**
     * @brief 清空图层
     */
    void ClearLayer();
private:
    ISceneGraph* m_pSceneGraph=nullptr;
};

#endif // CPLOTMAP_H
