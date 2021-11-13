#ifndef INTERFACE_MAP_LINE_HEARDER_H
#define INTERFACE_MAP_LINE_HEARDER_H

#include <Plot/Map/IMapSceneNode.h>
#include <Plot/SceneShape/ILine.h>
/**
 * @brief 场景节点类
 */
class IMapLine:public IMapSceneNode
{
public:
    CONSTRUCTOR(IMapLine,IMapSceneNode)

    /**
     * @brief 获取绘制信息
     * @return
     */
    ILine* GetDrawLine(){return(m_pDrawLine);}
protected:
    virtual ~IMapLine(){}
protected:
    ILine*  m_pDrawLine{};
};

#endif // INTERFACE_MAP_LINE_HEARDER_H
