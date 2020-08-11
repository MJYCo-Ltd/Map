#include <Inner/ILoadResource.h>
#include <osgText/Font>
#include "QtViewHud.h"
QtViewHud::QtViewHud(osg::View *pView)
    :m_pView(pView)
{
    m_pControlCanvas = osgEarth::Controls::ControlCanvas::getOrCreate(m_pView.get());

    auto pLabel = new osgEarth::Controls::LabelControl("Hello world\n I'm fine");
    //pLabel->setFont(osgText::readFontFile("E:/Git/Bin/fonts/msyh.ttf"));
    //pLabel->setPosition(50,50);
//    pLabel->setMargin(osgEarth::Controls::Control::SIDE_TOP,10);
    pLabel->setAlign(osgEarth::Controls::Control::ALIGN_CENTER,
                     osgEarth::Controls::Control::ALIGN_BOTTOM);
    m_pControlCanvas->addControl(pLabel);
}

QtViewHud::~QtViewHud()
{
    int n=0;
    ++n;
}
