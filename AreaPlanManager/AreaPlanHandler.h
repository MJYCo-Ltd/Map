#pragma once
#include <osgGA/GUIEventHandler>		 

class AreaPlanLayer;
class AreaPlanHandler : public osgGA::GUIEventHandler
{
public:
	AreaPlanHandler();
	~AreaPlanHandler();

	void setAreaPlanLayer(AreaPlanLayer* layer);
	void setEnable(bool isEnable);
	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

private:
	osg::Vec3d pickPos(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

private:
	bool				_isEnable;
	osg::Vec3d			_posRelease;
	AreaPlanLayer*		_areaPlanLayer;
};