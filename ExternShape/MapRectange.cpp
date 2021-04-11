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
    if(m_stPos1.fX < m_stPos2.fX)
    {
        if(m_stPos1.fY>m_stPos2.fY)         //左上
        {
            tPos_2.fX = m_stPos2.fX;
            tPos_2.fY = m_stPos1.fY;
            tPos_2.fZ =m_stPos1.fZ;

            tPos_4.fX = m_stPos1.fX;
            tPos_4.fY = m_stPos2.fY;
            tPos_4.fZ =m_stPos1.fZ;
        }
        else                //左下
        {
            tPos_4.fX = m_stPos1.fX;
            tPos_4.fY = m_stPos2.fY;
            tPos_4.fZ =m_stPos1.fZ;

            tPos_2.fX = m_stPos2.fX;
            tPos_2.fY = m_stPos1.fY;
            tPos_2.fZ =m_stPos1.fZ;
        }
    }
    else
    {

        if(m_stPos1.fY>m_stPos2.fY)     //右上
        {
            tPos_2.fX = m_stPos1.fX;
            tPos_2.fY = m_stPos2.fY;
            tPos_2.fZ =m_stPos1.fZ;

            tPos_4.fX = m_stPos2.fX;
            tPos_4.fY = m_stPos1.fY;
            tPos_4.fZ =m_stPos1.fZ;
        }
        else
        {
            tPos_2.fX = m_stPos1.fX;
            tPos_2.fY = m_stPos2.fY;
            tPos_2.fZ =m_stPos1.fZ;

            tPos_4.fX = m_stPos2.fX;
            tPos_4.fY = m_stPos1.fY;
            tPos_4.fZ =m_stPos1.fZ;
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
