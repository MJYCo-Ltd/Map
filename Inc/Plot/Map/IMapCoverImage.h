#ifndef INTERFACE_MAP_COVER_IMAGE_HEARDER_H
#define INTERFACE_MAP_COVER_IMAGE_HEARDER_H
#include <vector>
#include <string>
#include <Plot/Map/IMapSceneNode.h>
#include <SceneGraph/ISceneNode.h>

/**
 * @brief 位置信息
 */
struct PosValue
{
    double dX{};
    double dY{};
    double dValue{};
};

/**
 * @brief 覆盖信息
 */
struct CoverInfo
{
    int    nXNum{};
    int    nYNum{};
    double dMin{};
    double dMax{};
    std::vector<PosValue>vPosInfo;
};

/**
 * @brief 测点信息
 */
struct CeDian
{
    double dLon{};
    double dLat{};
    std::string sName;
};

/**
 * @brief 绘制地面覆盖类
 */
class IMapCoverImage:public IMapSceneNode
{
public:
    CONSTRUCTOR(IMapCoverImage,IMapSceneNode)

    /**
     * @brief 获取覆盖范围
     * @return
     */
    virtual void SetCoverInfo(const CoverInfo&)=0;

    /**
     * @brief 设置测点信息
     */
    virtual void SetCeDian(const std::vector<CeDian>&)=0;

    /**
     * @brief 设置最小值的颜色
     */
    virtual void SetMinValueColor(const SceneColor&)=0;

    /**
     * @brief 设置最大值的颜色
     */
    virtual void SetMaxValueColor(const SceneColor&)=0;

    /**
     * @brief 设置范围和图片
     */
    virtual void SetBound(const ScenePos&,const ScenePos&,const std::string&)=0;
protected:
    virtual ~IMapCoverImage(){}
};

#endif//INTERFACE_MAP_COVER_IMAGE_HEARDER_H
