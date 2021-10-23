#ifndef INTERSECTIONPICK_H
#define INTERSECTIONPICK_H
#include <QObject>
#include "PickTool.h"

class CToolSelector;
class ObjectIntersectionHandle;
class IntersectionPick: public QObject,public IToolBase
{
    Q_OBJECT
public:
    IntersectionPick(ISceneGraph *pSceneGraph, CToolSelector *pToolSelector);
    /**
     * @brief 初始化工具
     */
    bool InitTool();
    void ReleaseTool();
public slots:
    void HitId(unsigned int,unsigned int);

protected:
    ObjectIntersectionHandle* mIntersectionHandle;
};

#endif // INTERSECTIONPICK_H
