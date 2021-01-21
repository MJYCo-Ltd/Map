#ifndef INTERFACE_POINT_PICK_HEADER_H
#define INTERFACE_POINT_PICK_HEADER_H

#include <Tool/IToolBase.h>

/**
 * @brief 分析类基类
 */
class IPointPick:public IToolBase
{
public:
    CONSTRUCTOR(IPointPick,IToolBase)

protected:
    virtual ~IPointPick(){}
};

#endif//INTERFACE_POINT_PICK_HEADER_H
