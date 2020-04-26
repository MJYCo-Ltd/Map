#ifndef INCLUDE_HEADER_SCENERENDER_H
#define INCLUDE_HEADER_SCENERENDER_H

namespace osg
{
    class Operation;
}

class IRender
{
public:
    virtual ~IRender(){}

    /**
     * @brief 添加更新操作
     * @param pUpdate
     */
    virtual void AddUpdateOperation(osg::Operation*)=0;
};

#endif
