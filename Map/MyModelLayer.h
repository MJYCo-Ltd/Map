#ifndef CMYMODELLAYER_H
#define CMYMODELLAYER_H

#include <osgEarth/VisibleLayer>

namespace osgEarth {
class AnnotationNode;
}

class CMyModelLayer:public osgEarth::VisibleLayer
{
public:
    CMyModelLayer();
    //! Adds an annotation to the layer
    void addChild(osgEarth::AnnotationNode*);

    //! Gets the group to which you can add annotations
    osg::Group* getGroup() const;

public: // Layer

    virtual osg::Node* getNode() const;

    virtual void init();

protected:

    /** dtor */
    virtual ~CMyModelLayer(){}

private:

    osg::ref_ptr<osg::Group> _root;

    void deserialize();
};

#endif // CMYMODELLAYER_H
