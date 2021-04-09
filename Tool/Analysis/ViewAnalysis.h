#ifndef CVIEWANALYSIS_H
#define CVIEWANALYSIS_H

#include "../IToolBase.h"
class CViewAnalysis:public IToolBase
{
public:
public:
    CViewAnalysis(ISceneGraph* pSceneGraph,CToolSelector*pToolSelector):IToolBase(pSceneGraph,pToolSelector){}

    /**
     * @brief 初始化工具
     */
    bool InitTool();
    void ReleaseTool();
};

#endif // CVIEWANALYSIS_H
