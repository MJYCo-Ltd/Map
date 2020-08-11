#ifndef H_HEADER_CSCENEMODEL_H
#define H_HEADER_CSCENEMODEL_H
#include <osgEarth/ModelNode>
#include <Map/Plot/IModel.h>
#include <Inner/QtDrawShape.h>

/**
 * @brief 模型
 */
class CSceneModel:public QtDrawShape<IModel>
{
public:
    CSceneModel(ISceneGraph*);

    /**
     * @brief 更新地图节点
     */
    void UpdateMapNode(osgEarth::MapNode*);

    /**
     * @brief 初始化节点
     */
    void InitSceneNode();

    /**
     * @brief 位置更新消息
     */
    void PosChanged();

    /**
     * @brief 初始化样式
     */
    void InitStyle();

    /**
     * @brief 设置二维和模型变小时显示的图片
     */
    void Set2DAndSamllPic(const string&);

    /**
     * @brief 设置模型路径
     */
    void SetModelPath(const string&);

    /**
     * @brief 设置图片和模型切换的方式
     */
    void SetPicModelChange(PIC_MODEL_CHANGE_TYPE,double);

    /**
     * @brief 是否开启自动缩放
     */
    virtual void SetAutoScal(bool);

    /**
     * @brief 设置缩放系数
     */
    virtual void SetScalBit(double);

    /**
     * @brief 设置姿态
     */
    virtual void SetYPR(double,double,double);

private:
    bool   m_bAutoScal=false;
    double m_dScalBit=1.0;
    double m_dChangePos=1e5;   /// 切换
    osg::ref_ptr<osg::LOD>            m_pLodNode;
    osg::ref_ptr<osgEarth::ModelNode> m_pModel;
    osgEarth::Style   m_stylePic;    /// 节点样式
};

#endif // CSCENEMODEL_H
