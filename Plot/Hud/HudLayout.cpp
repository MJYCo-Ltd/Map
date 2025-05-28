#include "HudLayout.h"
#include <Plot/Map/IMap.h>
#include <iostream>
#include <SceneGraph/IWindow.h>
#include <SceneGraph/IViewPort.h>
#include "Inner/IOsgViewPoint.h"
#include <Hud/IViewHud.h>

namespace
{
    CHudLayout* S_pHudLaytout = nullptr;
}

bool operator==(osg::Vec3Array const& p1 ,osg::Vec3Array const& p2)
{
    if(p1.asVector().size() != p2.asVector().size())
    {
        return true;
    }
    for(int i=0; i < p1.asVector().size(); i++)
    {
        if(p1.asVector()[i].x() != p2.asVector()[i].x()
            || p1.asVector()[i].y() != p2.asVector()[i].y()
            || p1.asVector()[i].z() != p2.asVector()[i].z() )
            return true;
    }
    return false;
}


class CVBox:public osgEarth::Controls::VBox
{
public:
    CVBox(CHudLayout* pHudLayout):osgEarth::Controls::VBox()
    {
        m_pHudLaytout = pHudLayout;
    }

protected:
    virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osgEarth::Controls::ControlContext& cx )
    {
        //只有左键拖拽lable
        if(ea.getEventType() == osgGA::GUIEventAdapter::PUSH && ea.getButtonMask() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
        {
            S_pHudLaytout = m_pHudLaytout;
            float canvasYtm = cx._vp->height() - (ea.getY() - cx._view->getCamera()->getViewport()->y());
            float canvasXtm = ea.getX() - cx._view->getCamera()->getViewport()->x();
            HudPos pos1 = m_pHudLaytout->GetHudPos();
            canvasX = pos1.nX - canvasXtm;
            canvasY = pos1.nY - canvasYtm;
            const_cast<ISceneGraph*>(m_pHudLaytout->GetBoundSceneGraph())->GetMainWindow()->GetMainViewPoint()->GetHud()->SetCurrentHaveSelect(true);
        }
        else if(ea.getEventType() == osgGA::GUIEventAdapter::RELEASE /*&& ea.getButtonMask() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON*/)
        {
            clearControlState();
        }
        else if(ea.getEventType() == osgGA::GUIEventAdapter::DOUBLECLICK)
        {
            for(auto index = m_pHudLaytout->m_pAllObserver.begin();index != m_pHudLaytout->m_pAllObserver.end();++index)
            {
                (*index)->DoubleClick(m_pHudLaytout);
            }
        }
        else if(ea.getEventType() == osgGA::GUIEventAdapter::SCROLL) //地图操作不截断
        {
            return false;
        }

        if(ea.getEventType() == osgGA::GUIEventAdapter::FRAME && S_pHudLaytout && S_pHudLaytout == m_pHudLaytout)
        {
            float canvasYtm = cx._vp->height() - (ea.getY() - cx._view->getCamera()->getViewport()->y());
            float canvasXtm = ea.getX() - cx._view->getCamera()->getViewport()->x();

            newPos.nX = canvasXtm + canvasX;
            newPos.nY = canvasYtm + canvasY;
            // if(m_pHudLaytout->ModelScenePos())
            // {
            //     IMap* pmap = const_cast<ISceneGraph*>(m_pHudLaytout->GetBoundSceneGraph())->GetMap();
            //     if(pmap)
            //     {
            //         ScenePos modpos;
            //         int sx = newPos.nX;
            //         int sy = newPos.nY;
            //         pmap->ConvertCoord(sx, sy, modpos,0);
            //         if(modpos.dX == 0 && modpos.dY == 0)
            //         {
            //             clearControlState();
            //             return true;
            //         }
            //         m_pHudLaytout->SetMoveModelScenePos(&modpos);

            //     }
            // }
            m_pHudLaytout->SetHudPos(newPos);
            if(!m_beMoved) m_beMoved = true;
            auto size = this->renderSize();
            for(auto index = m_pHudLaytout->m_pAllObserver.begin();index != m_pHudLaytout->m_pAllObserver.end();++index)
            {
                (*index)->Move(m_pHudLaytout,newPos,size.x(),size.y());
            }

            // aa.requestRedraw();
            return(true);
        }
        else if(ea.getEventType() == osgGA::GUIEventAdapter::FRAME && m_pHudLaytout)
        {
            IMap* pmap = const_cast<ISceneGraph*>(m_pHudLaytout->GetBoundSceneGraph())->GetMap();
            if(pmap)
            {
                if(!m_beMoved)
                {
                    ScenePos * mpos = nullptr;
                    if(m_pHudLaytout->ModelScenePos())
                    {
                        mpos = m_pHudLaytout->ModelScenePos();
                    }
                    if(mpos)//= pModelNode->m_pMapLocationPos->GeoPos();
                    {
                        // HudPos ScreenPos;
                        ScenePos modpos = *mpos;
                        int sx, sy;
                        pmap->ConvertCoord(sx, sy, modpos,1);
                        HudPos frampos;
                        frampos.nX = sx;
                        frampos.nY = sy;
                        m_pHudLaytout->SetHudPos(frampos);
                    }
                }
                //划线
                if(m_beMoved && m_pHudLaytout->ModelScenePos())
                {

                    if(!m_pHudLaytout->m_line)
                    {
                        m_pHudLaytout->m_line = new osg::Geometry;
                        osg::Vec3Array* lcoords = new osg::Vec3Array;
                        dynamic_cast<osg::Geometry *>(m_pHudLaytout->m_line)->setVertexArray(lcoords);
                        dynamic_cast<osg::Geometry *>(m_pHudLaytout->m_line)->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));
                        m_pHudLaytout->m_pLayout->addChild(m_pHudLaytout->m_line);
                    }
                    if(osg::Geometry *line = dynamic_cast<osg::Geometry *>(m_pHudLaytout->m_line))
                    {
                        ScenePos modpos1 = *m_pHudLaytout->ModelScenePos();
                        int sx1, sy1;
                        float vph = cx._vp->height();
                        pmap->ConvertCoord(sx1, sy1, modpos1,1);
                        osg::Vec3Array* lcoords = new osg::Vec3Array;
                        lcoords->push_back(osg::Vec3(sx1,vph-sy1,0.0f));
                        lcoords->push_back(osg::Vec3(newPos.nX,vph-newPos.nY,0.0f));
                        if(*((osg::Vec3Array *)line->getVertexArray()) == *lcoords)
                        {
                            line->setVertexArray(lcoords);
                        }
                    }
                }
            }
            else if(m_pHudLaytout->ModelScenePos())//没有地图时设置位置
            {
                if(auto pView = dynamic_cast<IOsgViewPoint*>(const_cast<ISceneGraph*>(m_pHudLaytout->GetBoundSceneGraph())->GetMainWindow()->GetMainViewPoint()))
                {
                    if(auto osgview = pView->GetOsgView())
                    {
                        osg::Viewport* pViewPort = osgview->getCamera()->getViewport();

                        osg::ref_ptr<osg::Camera> camera = osgview->getCamera();
                        osg::Matrix mVPW = camera->getViewMatrix() * camera->getProjectionMatrix() * camera->getViewport()->computeWindowMatrix();
                        osg::Vec3 vecpos(m_pHudLaytout->ModelScenePos()->dX, m_pHudLaytout->ModelScenePos()->dY, m_pHudLaytout->ModelScenePos()->dZ);
                        vecpos = vecpos * mVPW;
                        float vph = cx._vp->height();
                        HudPos frampos;
                        frampos.nX = vecpos.x();
                        frampos.nY = vph-vecpos.y();
                        if(!m_beMoved)
                        {
                            m_pHudLaytout->SetHudPos(frampos);
                        }
                        else
                        {
                            if(!m_pHudLaytout->m_line)
                            {
                                m_pHudLaytout->m_line = new osg::Geometry;
                                osg::Vec3Array* lcoords = new osg::Vec3Array;
                                dynamic_cast<osg::Geometry *>(m_pHudLaytout->m_line)->setVertexArray(lcoords);
                                dynamic_cast<osg::Geometry *>(m_pHudLaytout->m_line)->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));
                                m_pHudLaytout->m_pLayout->addChild(m_pHudLaytout->m_line);
                            }
                            if(osg::Geometry *line = dynamic_cast<osg::Geometry *>(m_pHudLaytout->m_line))
                            {
                                osg::Vec3Array* lcoords = new osg::Vec3Array;
                                lcoords->push_back(osg::Vec3(vecpos.x(),vecpos.y(),0.0f));
                                lcoords->push_back(osg::Vec3(newPos.nX,vph-newPos.nY,0.0f));
                                if(*((osg::Vec3Array *)line->getVertexArray()) == *lcoords)
                                {
                                    line->setVertexArray(lcoords);
                                }
                            }

                        }
                    }
                }

            }
        }
        else if(ea.getEventType() == osgGA::GUIEventAdapter::DRAG && !S_pHudLaytout)
        {
            return false;
        }
        // return false;
        // if(S_pHudLaytout && S_pHudLaytout != m_pHudLaytout)
        // {
        //     return false;
        // }
        return osgEarth::Controls::Container::handle(ea,aa,cx);
    }
private:
    void clearControlState()
    {
        S_pHudLaytout = nullptr;
        canvasX = canvasY = 0;
        if(m_pHudLaytout)
        {
            const_cast<ISceneGraph*>(m_pHudLaytout->GetBoundSceneGraph())->GetMainWindow()->GetMainViewPoint()->GetHud()->SetCurrentHaveSelect(false);
        }
    }
private:
    CHudLayout* m_pHudLaytout;
    float canvasX, canvasY;
    bool m_beMoved = false;
    HudPos newPos;
    HudPos lastPos;
};

bool CHudLayout::AddHudNode(IHudNode *pNode)
{
    if (nullptr == pNode)
    {
        return(false);
    }

    /// 如果两者不是同一个渲染器,则返回
    if (pNode->GetBoundSceneGraph() != m_pSceneGraph)
    {
        return(false);
    }

    IOsgSceneNode* pOsgSceneNode = pNode->AsOsgSceneNode();

    auto findItor = std::find(m_pAllChild.begin(),m_pAllChild.end(),pOsgSceneNode);
    if (findItor == m_pAllChild.end())
    {
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(
                    new CAddControl2Container(
                        m_pLayout.get(),
                        dynamic_cast<osgEarth::Controls::Control*>(pOsgSceneNode->GetRealNode())));
        m_pAllChild.push_back(pOsgSceneNode);
        return(true);
    }

    return(true);
}

bool CHudLayout::RemoveHudNode(IHudNode *pNode)
{
    if (nullptr == pNode)
    {
        return(false);
    }

    IOsgSceneNode* pOsgSceneNode = pNode->AsOsgSceneNode();
    auto findItor = std::find(m_pAllChild.begin(),m_pAllChild.end(),pOsgSceneNode);
    if (findItor != m_pAllChild.end())
    {
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(
                    new CClearContainer(
                        m_pLayout.get()));

        osgEarth::Controls::ControlVector vControl;
        for(auto one : m_pAllChild)
        {
            if(one != pOsgSceneNode)
            {
                vControl.push_back(dynamic_cast<osgEarth::Controls::Control*>(one->GetRealNode()));
            }
        }
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(
                    new CAddControls2Container(
                        m_pLayout.get(),
                        vControl));

        pNode->HudTypeChanged();
    }

    return(true);
}

bool CHudLayout::SubObserver(const IHudLayoutMouseObserver *pObserver)
{
    if (nullptr == pObserver)
    {
        return(false);
    }

    auto findItor = std::find(m_pAllObserver.begin(),m_pAllObserver.end(),pObserver);
    if (findItor == m_pAllObserver.end())
    {
        m_pAllObserver.push_back(pObserver);
        return(true);
    }
    return(false);
}

bool CHudLayout::UnSubObserver(const IHudLayoutMouseObserver *pObserver)
{
    if (nullptr == pObserver)
    {
        return(false);
    }

    auto findItor = std::find(m_pAllObserver.begin(),m_pAllObserver.end(),pObserver);
    if (findItor == m_pAllObserver.end())
    {
        return(false);
    }

    m_pAllObserver.erase(findItor);
    return(true);
}

std::list<IHudNode *> CHudLayout::GetAllChildrenNode()
{
    std::list<IHudNode *> retlist;
    for(auto one : m_pAllChild)
    {
        if(IHudNode * node1 = dynamic_cast<IHudNode *>(one))
            retlist.push_back(node1);
    }
    return retlist;
}

void CHudLayout::InitNode()
{
    ImplHudNode<IHudLayout>::InitNode();
    m_pLayout = new osgEarth::Controls::HBox;
    m_pLayout->setBackColor(m_stColor.fR,m_stColor.fG,m_stColor.fB,m_stColor.fA);
    m_pLayout->setBorderColor(m_stOutLineColor.fR,m_stOutLineColor.fG,m_stOutLineColor.fB,m_stOutLineColor.fA);
    SetControlNode(m_pLayout.get());
}

/// 更新节点
void CHudLayout::FrameCall()
{
    if(m_bLayoutChanged)
    {
        osgEarth::Controls::Container* pContainer=nullptr;
        if(LAYOUT_VERTICAL == m_emType)
        {
            pContainer = new CVBox(this);
        }
        else
        {
            pContainer = new osgEarth::Controls::HBox;
        }
        pContainer->setBackColor(m_stColor.fR,m_stColor.fG,m_stColor.fB,m_stColor.fA);
        pContainer->setBorderColor(m_stOutLineColor.fR,m_stOutLineColor.fG,m_stOutLineColor.fB,m_stOutLineColor.fA);

        osgEarth::Controls::ControlVector vControl;
        for(auto one : m_pAllChild)
        {
            vControl.push_back(dynamic_cast<osgEarth::Controls::Control*>(one->GetRealNode()));
        }
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(
                    new CAddControls2Container(
                        pContainer,
                        vControl));

        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(
                    new COSGAttachNode(
                        m_pLayout.get(),
                        pContainer,true));

        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(
                    new COSGAttachNode(
                        m_pLayout.get(),
                        m_pLayout.get(),false));

        m_pLayout = pContainer;
        m_bLayoutChanged = false;

        SetControlNode(m_pLayout.get());
        m_bPosChanged=true;
        m_bTypeChanged=true;
        NodeChanged();
    }
    else if(m_bColorChanged)
    {
        m_pLayout->setBackColor(m_stColor.fR,m_stColor.fG,m_stColor.fB,m_stColor.fA);
        m_bColorChanged = false;
        NodeChanged();
    }
    else if(m_bOutLineColorChanged)
    {
        m_pLayout->setBorderColor(m_stOutLineColor.fR,m_stOutLineColor.fG,m_stOutLineColor.fB,m_stOutLineColor.fA);
        m_bOutLineColorChanged = false;
        NodeChanged();
    }
    else
    {
        ImplHudNode<IHudLayout>::FrameCall();
    }
}
