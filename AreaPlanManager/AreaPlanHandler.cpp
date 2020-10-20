#include "AreaPlanHandler.h"
#include "AreaPlanLayer.h"
#include "MeCore/MEMath.h"
#include <osgViewer/View>

AreaPlanHandler::AreaPlanHandler()
{
	_areaPlanLayer = nullptr;
	_isEnable = false;
}

AreaPlanHandler::~AreaPlanHandler()
{}

void AreaPlanHandler::setAreaPlanLayer(AreaPlanLayer* areaPlanLayer)
{
	_areaPlanLayer = areaPlanLayer;
}

void AreaPlanHandler::setEnable(bool isEnable)
{
	_isEnable = isEnable;
}

osg::Vec3d AreaPlanHandler::pickPos(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	osg::Vec3d worldPos;
	osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
	if (NULL == view)
	{
		return worldPos;
	}

	osgUtil::LineSegmentIntersector::Intersections intersections;
	if (view->computeIntersections(ea.getX(), ea.getY(), intersections))
	{
		auto iter = intersections.begin();
		for (; iter != intersections.end(); iter++)
		{
			worldPos = iter->getWorldIntersectPoint();
			return worldPos;
		}
	}
	return worldPos;
}

bool AreaPlanHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	if (_areaPlanLayer == nullptr)
		return false;
	if (!_isEnable)
	{
		if (_areaPlanLayer)
			_areaPlanLayer->finish();
		return false;
	}

	switch (ea.getEventType())
	{
		case osgGA::GUIEventAdapter::MOVE:
		{
			osg::Vec3d movePos = pickPos(ea, aa);
			_areaPlanLayer->moveEvent(MEMath::WCSToGCS(movePos));
			break;
		}
		case osgGA::GUIEventAdapter::RELEASE:
		{
			_posRelease = pickPos(ea, aa);

			if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
			{
				_areaPlanLayer->clickEvent(MEMath::WCSToGCS(_posRelease));
				return true;
			}
			else if (ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
			{
				_areaPlanLayer->rightEvent(MEMath::WCSToGCS(_posRelease));
				return true;
			}
			break;
		}
		default :
			break;
	}
	return false;
}
