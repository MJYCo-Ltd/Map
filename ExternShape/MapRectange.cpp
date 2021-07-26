#include <SceneGraph/ISceneGraph.h>
#include <Plot/IPlot.h>
#include <ExternShape/MapRectange.h>

CMapRectange::CMapRectange(ISceneGraph *pSceneGraph):
    m_pSceneGraph(pSceneGraph)
{
    InitNode();
}

/// 设置左上角
void CMapRectange::SetPos1(const ScenePos &pos)
{
    if(pos != m_stPos1)
    {
        m_stPos1 = pos;
        UpdateNode();
    }
}

/// 设置右下角
void CMapRectange::SetPos2(const ScenePos &pos)
{
    if(pos != m_stPos2)
    {
        m_stPos2 = pos;
        UpdateNode();
    }
}
void CMapRectange::SetColor(const SceneColor& color)
{
    m_pPolygon->GetDrawPolygon()->SetColor(color);
}
/// 初始化节点
void CMapRectange::InitNode()
{
    m_pPolygon = dynamic_cast<IMapPolygon*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapPolygon"));
}

/// 更新节点
void CMapRectange::UpdateNode()
{
    ScenePos tPos_2,tPos_4;
    if(m_stPos1.dX < m_stPos2.dX)
    {
        if(m_stPos1.dY>m_stPos2.dY)         //左上
        {
            tPos_2.dX = m_stPos2.dX;
            tPos_2.dY = m_stPos1.dY;
            tPos_2.dZ =m_stPos1.dZ;

            tPos_4.dX = m_stPos1.dX;
            tPos_4.dY = m_stPos2.dY;
            tPos_4.dZ =m_stPos1.dZ;
        }
        else                //左下
        {
            tPos_4.dX = m_stPos1.dX;
            tPos_4.dY = m_stPos2.dY;
            tPos_4.dZ =m_stPos1.dZ;

            tPos_2.dX = m_stPos2.dX;
            tPos_2.dY = m_stPos1.dY;
            tPos_2.dZ =m_stPos1.dZ;
        }
    }
    else
    {

        if(m_stPos1.dY>m_stPos2.dY)     //右上
        {
            tPos_2.dX = m_stPos1.dX;
            tPos_2.dY = m_stPos2.dY;
            tPos_2.dZ =m_stPos1.dZ;

            tPos_4.dX = m_stPos2.dX;
            tPos_4.dY = m_stPos1.dY;
            tPos_4.dZ =m_stPos1.dZ;
        }
        else
        {
            tPos_2.dX = m_stPos1.dX;
            tPos_2.dY = m_stPos2.dY;
            tPos_2.dZ =m_stPos1.dZ;

            tPos_4.dX = m_stPos2.dX;
            tPos_4.dY = m_stPos1.dY;
            tPos_4.dZ =m_stPos1.dZ;
        }
    }

    if(m_pPolygon->GetDrawPolygon()->GetCount() ==0)
    {
        m_pPolygon->GetDrawPolygon()->AddPoint(0,m_stPos1);
        m_pPolygon->GetDrawPolygon()->AddPoint(1,tPos_2);
        m_pPolygon->GetDrawPolygon()->AddPoint(2,m_stPos2);
        m_pPolygon->GetDrawPolygon()->AddPoint(3,tPos_4);
    }
    else
    {
        m_pPolygon->GetDrawPolygon()->UpdatePoint(0,m_stPos1);
        m_pPolygon->GetDrawPolygon()->UpdatePoint(1,tPos_2);
        m_pPolygon->GetDrawPolygon()->UpdatePoint(2,m_stPos2);
        m_pPolygon->GetDrawPolygon()->UpdatePoint(3,tPos_4);

    }
}
