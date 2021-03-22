#include <SceneGraph/ISceneGraph.h>
#include <Plot/IPlot.h>
#include <ExternShape/MapRectange.h>

CMapRectange::CMapRectange(ISceneGraph *pSceneGraph):
    m_pSceneGraph(pSceneGraph)
{
    InitNode();
}

/// 设置左上角
void CMapRectange::SetLeftUp(const MapGeoPos &rLeftUp)
{
    if(rLeftUp != m_stLeftUp)
    {
        m_stLeftUp = rLeftUp;
        UpdateNode();
    }
}

/// 设置右下角
void CMapRectange::SetRightDown(const MapGeoPos &rRightDown)
{
    if(rRightDown != m_stRightDown)
    {
        m_stRightDown = rRightDown;
        UpdateNode();
    }
}

/// 初始化节点
void CMapRectange::InitNode()
{
    m_pPolygon = dynamic_cast<IMapPolygon*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapPolygon"));
}

/// 更新节点
void CMapRectange::UpdateNode()
{
    double dLeftLon=0;
    if(m_stRightDown.fLon>0 && m_stLeftUp.fLon<m_stRightDown.fLon)
    {
        dLeftLon = m_stLeftUp.fLon + 360;
    }


}
