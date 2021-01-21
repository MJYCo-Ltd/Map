#ifndef CPOINTPICK_H
#define CPOINTPICK_H
#include <osgEarth/RTTPicker>
#include <Tool/Pick/IPointPick.h>

class CPointPick:public IPointPick
{
public:
    CONSTRUCTOR(CPointPick,IPointPick)

    /**
     * @brief 初始化工具
     */
    void InitTool();
    void ReleaseTool();
protected:
    osg::ref_ptr<osgEarth::Util::RTTPicker> m_pPicker;
};

#endif // CPOINTPICK_H
