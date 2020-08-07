#ifndef INTERFACE_DRAWSHAPE_HEARDER_H
#define INTERFACE_DRAWSHAPE_HEARDER_H
#include "../IMapSceneNode.h"

/**
 * @brief 场景节点类
 */
class IDrawShape:public IMapSceneNode
{
public:
    virtual ~IDrawShape(){}

    /**
     * @brief 判断是id是否相同
     * @return
     */
    virtual bool JudgeID(unsigned)=0;
};

#endif
