#ifndef CMAP_EXTERN_LINE_H
#define CMAP_EXTERN_LINE_H
#include <ExternShape/ExternShape_Global.h>
#include <Plot/Map/IMapLine.h>

/**
 * @brief 在地图上绘制矩形
 */
class OSGEXTERNSHARED_EXPORT CMapExternLine
{
public:
    CMapExternLine(ISceneGraph* pSceneGraph);

    bool AddPoint(int,const ScenePos&);
    bool UpdatePoint(int,const ScenePos&);
    bool DelPoint(int);
    void SetMultPos(const std::list<ScenePos>&);

    /**
     * @brief 设置细分参数
     * @param dTessellation [m]
     * @return
     */
    void SetTessellation(double dTessellation)JUDGE_DOUBLE_CALL_FUNCTION(dTessellation,m_dTessellation,PointUpdate)
    double Tessellation(){return(m_dTessellation);}
protected:
    /**
     * @brief 点更新
     */
    void PointUpdate();
protected:
    ISceneGraph*  m_pSceneGraph{};
    IMapLine*     m_pMapLine{};
    double        m_dTessellation{1000.};
    std::list<ScenePos>   m_listAllPos;
    std::vector<ScenePos> m_listVirtualPos;
};

#endif // CMAP_EXTERN_LINE_H
