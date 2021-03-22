#ifndef CPOINTPICK_H
#define CPOINTPICK_H
#include <QObject>
#include <osgEarth/RTTPicker>
#include "PickTool.h"

class CToolSelector;
class CPointPick:public QObject,public IToolBase
{
    Q_OBJECT
public:
    CPointPick(ISceneGraph* pSceneGraph,CToolSelector*pToolSelector);

    /**
     * @brief 初始化工具
     */
    bool InitTool();
    void ReleaseTool();

public slots:
    void HitId(unsigned int,unsigned int);
protected:
    osg::ref_ptr<osgEarth::Util::RTTPicker> m_pPicker;
};

#endif // CPOINTPICK_H
