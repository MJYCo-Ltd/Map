#include <SceneGraph/ISceneGraph.h>
#include <Plot/IPlot.h>
#include <ExternShape/MapRectange.h>

CMapRectange::CMapRectange(ISceneGraph *pSceneGraph):
    m_pSceneGraph(pSceneGraph)
{
    InitNode();
}

/// 设置左上角
void CMapRectange::SetPos1(const MapGeoPos &pos)
{
    if(pos != m_stPos1)
    {
        m_stPos1 = pos;
        UpdateNode();
    }
}

/// 设置右下角
void CMapRectange::SetPos2(const MapGeoPos &pos)
{
    if(pos != m_stPos2)
    {
        m_stPos2 = pos;
        UpdateNode();
    }
}
void CMapRectange::SetColor(const SceneColor& color)
{
    m_pPolygon->SetPolygonColor(color);
}
/// 初始化节点
void CMapRectange::InitNode()
{
    m_pPolygon = dynamic_cast<IMapPolygon*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapPolygon"));
}

/// 更新节点
void CMapRectange::UpdateNode()
{
    MapGeoPos tPos_2,tPos_4;
    if(m_stPos1.fLon < m_stPos2.fLon)
    {
        if(m_stPos1.fLat>m_stPos2.fLat)         //左上
        {
            tPos_2.fLon = m_stPos2.fLon;
            tPos_2.fLat = m_stPos1.fLat;
            tPos_2.fHeight =m_stPos1.fHeight;

            tPos_4.fLon = m_stPos1.fLon;
            tPos_4.fLat = m_stPos2.fLat;
            tPos_4.fHeight =m_stPos1.fHeight;
        }
        else                //左下
        {
            tPos_4.fLon = m_stPos1.fLon;
            tPos_4.fLat = m_stPos2.fLat;
            tPos_4.fHeight =m_stPos1.fHeight;

            tPos_2.fLon = m_stPos2.fLon;
            tPos_2.fLat = m_stPos1.fLat;
            tPos_2.fHeight =m_stPos1.fHeight;
        }
    }
    else
    {

        if(m_stPos1.fLat>m_stPos2.fLat)     //右上
        {
            tPos_2.fLon = m_stPos1.fLon;
            tPos_2.fLat = m_stPos2.fLat;
            tPos_2.fHeight =m_stPos1.fHeight;

            tPos_4.fLon = m_stPos2.fLon;
            tPos_4.fLat = m_stPos1.fLat;
            tPos_4.fHeight =m_stPos1.fHeight;
        }
        else
        {
            tPos_2.fLon = m_stPos1.fLon;
            tPos_2.fLat = m_stPos2.fLat;
            tPos_2.fHeight =m_stPos1.fHeight;

            tPos_4.fLon = m_stPos2.fLon;
            tPos_4.fLat = m_stPos1.fLat;
            tPos_4.fHeight =m_stPos1.fHeight;
        }
    }

    if(m_pPolygon->GetPointCount() ==0)
    {
        m_pPolygon->AddPoint(0,m_stPos1);
        m_pPolygon->AddPoint(1,tPos_2);
        m_pPolygon->AddPoint(2,m_stPos2);
        m_pPolygon->AddPoint(3,tPos_4);
    }
    else
    {
        m_pPolygon->UpdatePoint(0,m_stPos1);
        m_pPolygon->UpdatePoint(1,tPos_2);
        m_pPolygon->UpdatePoint(2,m_stPos2);
        m_pPolygon->UpdatePoint(3,tPos_4);

    }
}
