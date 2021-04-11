#ifndef CSCENECOVER_H
#define CSCENECOVER_H
#include <osgEarth/DrapeableNode>
#include <osgEarth/ImageOverlay>
#include <Plot/Map/IMapCoverImage.h>
#include <Inner/ImplMapSceneNode.hpp>

class CMapCoverImage:public ImplMapSceneNode<IMapCoverImage>
{
public:
    CONSTRUCTOR(CMapCoverImage,ImplMapSceneNode<IMapCoverImage>)

    /**
     * @brief 更新地图节点
     * @param pMapNode
     */
    void UpdateMapNode();

    /**
     * @brief 初始化节点
     */
    void InitNode();


    /**
     * @brief 获取覆盖范围
     * @return
     */
    virtual void SetCoverInfo(const CoverInfo&);

    /**
     * @brief 设置测点信息
     */
    void SetCeDian(const std::vector<CeDian>&);

    /**
     * @brief 初始化样式
     */
    void InitStyle();

    /**
     * @brief 设置最小颜色
     */
    void SetMinValueColor(const SceneColor&);

    /**
     * @brief 设置最大值颜色
     */
    void SetMaxValueColor(const SceneColor&);

    void SetBound(const ScenePos&,const ScenePos&,const std::string&);

private:
    std::vector<CeDian> m_vCedian;    //// 测点信息
    CoverInfo      m_rCoverInfo; /// 覆盖信息
    SceneColor     m_stMinColor;
    SceneColor     m_stMaxColor;
    osg::ref_ptr<osgEarth::DrapeableNode> m_pDrapeableNode;
    osg::ref_ptr<osgEarth::ImageOverlay> m_pImage;
    osg::ref_ptr<osg::Geometry> m_pCoverage;
    osg::observer_ptr<osg::Group> m_pCoverRoot;
};

#endif // CSCENECOVER_H
