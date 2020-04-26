#ifndef MILKYWAY_H
#define MILKYWAY_H

#include <osg/Drawable>
#include <osg/ShapeDrawable>
#include <osg/Geode>
class ISceneGraph;


class CMilkyway:public osg::Referenced
{
public:
    CMilkyway(ISceneGraph* pSceneGraph);

    osg::Node* getNode();
protected:
    ~CMilkyway();

private:
    void makeMilkyway();

private:
    ISceneGraph* m_pSceneGraph;
    osg::ref_ptr<osg::ShapeDrawable> m_pMilkyway;
};

#endif // MILKYWAY_H
