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
    /**
     * @brief 添加位置点
     * @return
     */
    virtual bool AddPoint(int,const ScenePos&)=0;
    /**
     * @brief 更新位置点
     * @return
     */
    virtual bool UpdatePoint(int,const ScenePos&)=0;
    /**
     * @brief 删除位置点
     * @return
     */
    virtual bool DelPoint(int,const ScenePos&)=0;
    /**
     * @brief 设置位置点
     * @return
     */
    virtual void SetMultPos(const std::list<ScenePos>&)=0;
    /**
     * @brief 设置细分参数
     * @return
     */
    void SetTessellation(int nTessellation ){m_dTessellation =nTessellation*1000.0; }
protected:
    virtual ~IMapLine(){}
protected:
    ILine*       m_pDrawLine{};
    bool        m_bPointChange={false};
    double m_dTessellation =0;
};

#endif // INTERFACE_MAP_LINE_HEARDER_H
