#ifndef INTERFACE_GEOMERTY_HEARDER_H
#define INTERFACE_GEOMERTY_HEARDER_H
#include <vector>
#include <list>
#include <Plot/SceneShape/IShape.h>

/**
 * @brief 点的位置处理器
 */
struct DealPoint
{
    /**
     * @brief 点位置坐标转换函数
     * @return
     */
    virtual bool Conversion(const std::vector<ScenePos>&, std::vector<ScenePos>&){return(false);}

    /**
     * @brief 几何图形形状改变消息
     */
    virtual void ShapeChanged(){}
};

/**
 * @brief 绘制几何图形
 * @attention 最多到面
 */
class IGeometry:public IShape
{
public:
    CONSTRUCTOR(IGeometry,IShape)

    /**
     * @brief 增加点
     */
    virtual void AddPoint(int,const ScenePos&)=0;
    virtual void AddPoint(const ScenePos&)=0;

    /**
     * @brief 移除点
     */
    virtual bool RemovePoint(int)=0;

    /**
     * @brief 更新指定位置点
     */
    virtual bool UpdatePoint(int,const ScenePos&)=0;

    /**
     * @brief 设置多个位置,会将原始数据清空
     */
    virtual void SetMultPos(const std::vector<ScenePos>&)=0;
    virtual std::vector<ScenePos> GetMulPos()const=0;

    /**
     * @brief 获取几何图形中点的个数
     * @return
     */
    virtual int GetCount()const=0;

    /**
     * @brief 获取指定位置的点
     * @attention 索引从0开始到Count-1
     * @attention 如果索引超过数据范围则返回 0,0,0
     * @attention 如果返回 0,0,0 切记检查是否越界
     * @return
     */
    virtual const ScenePos& GetPoint(int)const=0;

    /**
     * @brief 清空所有的点
     */
    virtual void Clear()=0;

    /**
     * @brief 调用会降低性能
     * @attention 内部调用
     */
    virtual void NeedUpdate()=0;

    /**
     * @brief 设置处理信息的点
     * @param pDealPoin
     */
    void SetDealPoint(DealPoint* pDealPoin)
    {
        m_pDealPoint=pDealPoin;
    }

protected:
    virtual ~IGeometry(){}
protected:
    DealPoint* m_pDealPoint{};
};

#endif//INTERFACE_POLYGON_HEARDER_H
