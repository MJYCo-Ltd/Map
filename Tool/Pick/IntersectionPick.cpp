#include "IntersectionPick.h"
#include <SceneGraph/ISceneGraph.h>
#include "../ToolSelector/ToolSelector.h"
#include <SceneGraph/IWindow.h>
#include <SceneGraph/IViewPort.h>
#include <Inner/IOsgViewPoint.h>
#include <Inner/IOsgSceneNode.h>
#include <Inner/IRender.h>
#include <Inner/OsgExtern/OsgExtern.h>
#include <Plot/Map/IMap.h>
#include <osgEarth/IntersectionPicker>
#include "../ToolSelector/ToolSelector.h"
#include <osgEarth/Registry>
class ObjectIntersectionHandle : public osgGA::GUIEventHandler
{
public:
    ObjectIntersectionHandle(osg::ref_ptr<osg::Group> p,IntersectionPick* picker):myScene(p),m_pPicker(picker)
    {

    }
    bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
    {
        unsigned m_unID =0;
        if (ea.getEventType() == osgGA::GUIEventAdapter::PUSH && ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
            m_EventType =1;
        else if (osgGA::GUIEventAdapter::DOUBLECLICK == ea.getEventType() && ea.getButtonMask() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
            m_EventType =2;
        else
            return false;
        osgViewer::View *view = dynamic_cast<osgViewer::View*>(aa.asView());

        osgUtil::LineSegmentIntersector::Intersections intersections;
        if (view->computeIntersections(ea.getX(), ea.getY(), intersections))
        {
         for (osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin(); hitr != intersections.end(); ++hitr)
         {
              const osg::NodePath& nodePath = hitr->nodePath;

              for(int i=0;i<nodePath.size();i++)
              {
                  osg::Node* node = nodePath.at(i);

                  osgEarth::Registry::instance()->getObjectIndex()->getObjectID(node,m_unID);
                  if(m_unID>0)
                      break ;
              }

         }
        }

         QMetaObject::invokeMethod(m_pPicker,"HitId",Q_ARG(unsigned int,m_unID),Q_ARG(unsigned int,m_EventType));
        return false;
    }
protected:
    unsigned  int m_EventType = 0;      //1 单击 2双击
    IntersectionPick* m_pPicker;
    osg::ref_ptr<osg::Group> myScene;
};

IntersectionPick::IntersectionPick(ISceneGraph *pSceneGraph, CToolSelector *pToolSelector):
IToolBase(pSceneGraph,pToolSelector){}

bool IntersectionPick::InitTool()
{
    auto pView = dynamic_cast<IOsgViewPoint*>(m_pSceneGraph->GetMainWindow()->GetMainViewPoint());
    osg::ref_ptr<osg::Group> mapNode = dynamic_cast<IOsgSceneNode*>(m_pSceneGraph->GetMap())->GetOsgNode()->asGroup();
    mIntersectionHandle = new ObjectIntersectionHandle(mapNode,this);
    pView->GetOsgView()->addEventHandler(mIntersectionHandle );
    return true;
}
void IntersectionPick::ReleaseTool()
{
    auto pView = dynamic_cast<IOsgViewPoint*>(m_pSceneGraph->GetMainWindow()->GetMainViewPoint());
    if(nullptr != pView)
    {
        pView->GetOsgView()->removeEventHandler(mIntersectionHandle);
    }
}
void IntersectionPick::HitId(unsigned int unID,unsigned int type)
{
    m_pToolSelector->PickID(unID,type);
}
