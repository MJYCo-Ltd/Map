#ifndef CSCENEIMAGE_H
#define CSCENEIMAGE_H

#include <Plot/SceneShape/IImage.h>
#include <Inner/ImplSceneNode.hpp>
class CSceneImage:public ImplSceneNode<IImage>
{
public:
    CONSTRUCTOR(CSceneImage,ImplSceneNode<IImage>)
protected:
    ~CSceneImage();
    void ImageSizeChanged(){if(m_stImageSize) m_stImageSize.bOutSet=true;NodeChanged();}
    void ImagePathChanged()SET_TRUE_NODE_UPDATE(m_bPathChanged)
    void ImageDataChanged()SET_TRUE_NODE_UPDATE(m_bImageDataChanged)
    void SetQImage(const QImage& rImage);

    void InitNode();
    /**
     * @brief 初始化节点
     */
    void ColorChanged(){NodeChanged();}
    /**
     * @brief实现父类方法
     */
    void ShapeChanged(){}
    /**
     * @brief 重写父类
     */
    void FrameCall();

    /**
     * @brief 获取影像节点
     * @return
     */
    osg::Geometry* TryChangeToGeomerty(osg::Node *pNode);

protected:
    bool m_bPathChanged{false};
    bool m_bImageDataChanged{false};
    bool m_bGeomertyChanged{false};
    osg::ref_ptr<osg::Geometry> m_pQImageDrawNode;
    static std::map<std::string,osg::observer_ptr<osg::Geometry>>s_mapID2ImageNode;
};

#endif // CSCENEIMAGE_H
