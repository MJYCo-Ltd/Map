#include <sstream>
#include <QString>
#include <osg/LineStipple>
#include <osgEarth/Registry>
#include <osg/DisplaySettings>
#include <ISceneCore.h>
#include "Boundary.h"



CBoundary::CBoundary()
{
    makeBoundray();
}

/// 设置是否显示
void CBoundary::SetVisible(bool bVisible)
{
    if(m_bShowBoundary == bVisible)
    {
        return;
    }

    m_bShowBoundary = bVisible;

    if(m_bShowBoundary)
    {
        m_pBoundray->setNodeMask(-1);
    }
    else
    {
        m_pBoundray->setNodeMask(0);
    }
}

osg::Node *CBoundary::getNode()
{
    return(m_pBoundray.get());
}

CBoundary::~CBoundary()
{
}

void CBoundary::makeBoundray()
{
    std::string sBoundName = GetDataPath() + "Space/stars/boundaries";
    FILE *file = fopen(sBoundName.c_str(), "rb");
    if (!file)
    {
        return;
    }
    qint32 count;

    m_pBoundray = new osg::Geometry;
    osg::Vec3Array *verts = new osg::Vec3Array;
    osg::Vec4Array *colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(0.8, 0.3, 0.3, 0.8));

    osg::MixinVector<GLushort> vIndex;
    GLushort unIndex=0;


    while (!feof(file))
    {
        if (fread(&count, sizeof(count), 1, file) != 1)
            continue;
        if (count <= 0)
        {
            osg::notify(osg::WARN) << "Failed when read the boundary data!"
                    << std::endl;
            continue;
        }

        double *data = new double[count * 2];
        if ((int) fread(data, sizeof(double), count * 2, file) != count * 2)
        {
            delete[] data;
            osg::notify(osg::WARN) << "Failed when read the boundary data!"
                    << std::endl;
            continue;
        }

        vIndex.clear();
        vIndex.reserve(count);



        double x, y, z;
        for (int i = 0; i < count; i++)
        {
            data[2 * i + 0] *= osg::PI / 12.0;
            data[2 * i + 1] *= osg::PI / 180.0;
            x = cos(data[2 * i + 0]) * cos(data[2 * i + 1]);
            y = sin(data[2 * i + 0]) * cos(data[2 * i + 1]);
            z = sin(data[2 * i + 1]);
            verts->push_back(osg::Vec3(x, y, z));
            vIndex.push_back(unIndex++);
        }

        /// 放入
        m_pBoundray->addPrimitiveSet(new osg::DrawElementsUShort(GL_LINE_STRIP,count,&vIndex.front()));

        delete[] data;
    }


    m_pBoundray->setVertexArray(verts);
    m_pBoundray->setColorArray(colors);
    m_pBoundray->setColorBinding(osg::Geometry::BIND_OVERALL);
    //m_pBoundray->setCullingActive(false);

    fclose(file);

    osg::StateSet *state = m_pBoundray->getOrCreateStateSet();
    state->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
}
