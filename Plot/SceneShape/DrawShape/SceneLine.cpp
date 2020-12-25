#include <osgEarth/CullingUtils>
#include "SceneLine.h"

struct MyDrawCallBack:public osg::Drawable::DrawCallback
{
    void drawImplementation(osg::RenderInfo& renderInfo,const osg::Drawable* drawable) const
    {
        drawable->drawImplementation(renderInfo);

        auto pLastProgram=renderInfo.getState()->getLastAppliedProgramObject();
        if(nullptr != pLastProgram)
        {

            auto pProgram = pLastProgram->getProgram();
            osg::notify(osg::WARN)<<"====================================================="<<std::endl;
            osg::notify(osg::WARN)<<pProgram->getName()<<"\n--------------------------------------------"<<std::endl;
            for(int i=0;i<pProgram->getNumShaders();++i)
            {
                osg::notify(osg::WARN)<<pProgram->getShader(i)->getName()<<"\n--------------------------------------------"<<std::endl;
                osg::notify(osg::WARN)<<pProgram->getShader(i)->getShaderSource()<<std::endl;
            }
        }
    }
};

/// 添加点
void CSceneLine::AddPoint(int nIndex, const ScenePos &rScenePos)
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

    ShapeChanged();
}

/// 移除指定位置点
bool CSceneLine::RemovePoint(int nIndex)
{
    if(nIndex<0 || nIndex >= m_listAllPos.size())
    {
        return(false);
    }

    auto pIter = m_listAllPos.begin();
    for(int i=0; i<nIndex;++i,++pIter){}
    m_listAllPos.erase(pIter);

    ShapeChanged();
    return(true);
}

/// 更新指定位置的
bool CSceneLine::UpdatePoint(int nIndex, const ScenePos &rPos)
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
        ShapeChanged();
    }

    return(true);
}

///初始化多个位置点
void CSceneLine::SetMultPos(const std::vector<ScenePos> & vAllPoints)
{
    m_listAllPos.clear();

    for(auto one : vAllPoints)
    {
        m_listAllPos.push_back(one);
    }

    ShapeChanged();
}

std::vector<ScenePos> CSceneLine::GetMulPos() const
{
    std::vector<ScenePos> vTempPos;
    vTempPos.resize(m_listAllPos.size());
    int nIndex(0);
    for(auto one : m_listAllPos)
    {
        ScenePos& rPos = vTempPos.at(nIndex++);

        rPos.fX = one.fX;
        rPos.fY = one.fY;
        rPos.fZ = one.fZ;
    }

    return(vTempPos);
}

/// 形状更改
void CSceneLine::ShapeChanged()
{
    m_bShapeChanged=true;
    ImplSceneNode<ILine>::NodeChanged();
}

/// 颜色更改
void CSceneLine::ColorChanged()
{
    m_bColorChanged=true;
    ImplSceneNode<ILine>::NodeChanged();
}

void CSceneLine::InitNode()
{
    osg::Group *pGroup = new osg::Group;
    m_pLine = new osgEarth::LineDrawable;
    pGroup->addChild(m_pLine);
    pGroup->addCullCallback(new osgEarth::InstallCameraUniform);
    m_pLine->setColor(osg::Vec4(m_stColor.fR,m_stColor.fG,m_stColor.fB,m_stColor.fA));
    m_pLine->setLineWidth(m_nLineWidth);
    m_pLine->setStipplePattern(0xffff);
    m_pLine->setStippleFactor(2);
    m_pLine->setLineSmooth(true);

    ImplSceneNode<ILine>::InitNode();
    ImplSceneNode<ILine>::SetOsgNode(pGroup);
}

void CSceneLine::UpdateNode()
{
    if(m_bColorChanged)
    {
        m_pLine->setColor(osg::Vec4(m_stColor.fR,m_stColor.fG,m_stColor.fB,m_stColor.fA));
        m_bColorChanged = false;
    }

    if(m_bShapeChanged)
    {
        m_pLine->clear();
        for(auto one : m_listAllPos)
        {
            m_pLine->pushVertex(osg::Vec3(one.fX,one.fY,one.fZ));
        }
        m_pLine->setFirst(0);
        m_pLine->finish();
        m_bShapeChanged=false;
    }

    ImplSceneNode<ILine>::UpdateNode();
}
