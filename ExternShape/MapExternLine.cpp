#include <VersionMathCommon.h>
#include <GisMath/GisMath.h>
#include <SceneGraph/ISceneGraph.h>
#include <Plot/IPlot.h>
#include <ExternShape/MapExternLine.h>

CMapExternLine::CMapExternLine(ISceneGraph *pSceneGraph):
    m_pSceneGraph(pSceneGraph)
{
    m_pMapLine = dynamic_cast<IMapLine*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLine"));
}

bool CMapExternLine::AddPoint(int nIndex, const ScenePos &rScenePos)
{
    if(nIndex <=0)
    {
        m_listAllPos.push_front(rScenePos);
    }
    else if(nIndex >= m_listAllPos.size())
    {
        m_listAllPos.push_back(rScenePos);
    }
    else
    {
        auto pIter = m_listAllPos.begin();
        for(int i=0; i<nIndex;++i,++pIter){}

        m_listAllPos.insert(pIter,rScenePos);
    }

    return(true);
}

bool CMapExternLine::UpdatePoint(int nIndex, const ScenePos &rPos)
{
    if(nIndex<0 || nIndex >= m_listAllPos.size())
    {
        return(false);
    }

    auto pIter = m_listAllPos.begin();
    for(int i=0; i<nIndex;++i,++pIter){}

    if(*pIter != rPos)
    {
        *pIter = rPos;
    }

    return(true);
}

bool CMapExternLine::DelPoint(int nIndex)
{
    if(nIndex<0 || nIndex >= m_listAllPos.size())
    {
        return(false);
    }

    auto pIter = m_listAllPos.begin();
    for(int i=0; i<nIndex;++i,++pIter){}
    m_listAllPos.erase(pIter);

    return(true);
}

void CMapExternLine::SetMultPos(const std::list<ScenePos> &posVec)
{
    m_listAllPos.clear();
    m_listAllPos =posVec;
}

void CMapExternLine::PointUpdate()
{
    m_listVirtualPos.clear();
    if(m_listAllPos.size()<=1 ||m_dTessellation <=1.0f)
    {
        auto iter = m_listAllPos.begin();
        while(iter != m_listAllPos.end())
        {
            m_listVirtualPos.push_back(*iter);
            iter++;
        }
    }
    else
    {
        ScenePos _tPos;
        auto iter = m_listAllPos.begin();
        _tPos = *iter;
        m_listVirtualPos.push_back(_tPos);
        iter++;
        double dAzim=0.0 ,dElev=0.0,dDist=0.0;
        while(iter != m_listAllPos.end())
        {
            GisMath::CalAzElGeo(_tPos.dX*DD2R,_tPos.dY*DD2R,_tPos.dZ,iter->dX*DD2R,iter->dY*DD2R,iter->dZ,dAzim,dElev,dDist);

            double dTessellation =dDist/m_dTessellation;
            if(dTessellation <1.1f)
            {
                _tPos = *iter;
                m_listVirtualPos.push_back(_tPos);
                iter++;
                continue;
            }
            double tmpElev = dTessellation>0?dElev/dTessellation:0;

            for(int i=0;i<dTessellation-1;i++)
            {
                double dLon,dLat,dHeight;
                GisMath::GeoCalEndGeo(_tPos.dX*DD2R,_tPos.dY*DD2R,_tPos.dZ,dAzim,tmpElev*(i+1),m_dTessellation*(i+1),dLon,dLat,dHeight);
                ScenePos _tmpPos;
                _tmpPos.dX =dLon*DR2D;
                _tmpPos.dY =dLat*DR2D;
                _tmpPos.dZ =dHeight;
                m_listVirtualPos.push_back(_tmpPos);
            }
            _tPos = *iter;
            m_listVirtualPos.push_back(_tPos);
            iter++;
        }
    }
    m_pMapLine->GetDrawLine()->SetMultPos(m_listVirtualPos);
}