#ifndef H_HEADER_CSCENEMODEL_H
#define H_HEADER_CSCENEMODEL_H
#include <osgEarth/ModelNode>
#include <osgEarth/PlaceNode>
#include <Map/Plot/IModel.h>
#include <Inner/QtDrawShape.h>

class ImageCallBack;
class ModelCallBack;

/**
 * @brief 模型
 */
class CSceneModel:public QtDrawShape<IModel>
{
    friend class ImageCallBack;
    friend class ModelCallBack;
public:
    CSceneModel(ISceneGraph*);

    /**
     * @brief 设置名称
     */
    void SetName(const string&);

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
    void Set2DAndSamllPic(const string&,double);

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
protected:
    /**
     * @brief 更改lod类型
     */
    void ChangeLodType();

    /**
     * @brief 更新缩放系数
     */
    void ChangeScal();

    /**
     * @brief 更新姿态角
     */
    void ChangeYPR();

private:
    bool   m_bAutoScal=false;
    bool   m_bUpdateImage=false;
    bool   m_bUpdateName=false;
    bool   m_bUpdateStle=false;
    bool   m_bUpdateModel=false;

    double m_dScalBit=1.0;
    double m_dChangePos=1e5;   /// 切换

    double m_dYaw=0.;
    double m_dPitch=0.;
    double m_dRoll=0.;
    double m_dInitAngle=0.;

    ScenePos m_prePos;

    string m_sPicPath;
    string m_sModelPath;
    string m_sName;
    PIC_MODEL_CHANGE_TYPE m_emType = EYE_DISTANCE;
    osg::ref_ptr<osg::LOD>            m_pLodNode;
    osg::ref_ptr<osgEarth::ModelNode> m_pModel;
    osg::ref_ptr<osgEarth::PlaceNode> m_pImage;
    osg::ref_ptr<ImageCallBack>       m_pImageCallBack;
    osg::ref_ptr<ModelCallBack>       m_pModelCallBack;
    osg::Image*                       m_pPic=nullptr;
    osgEarth::Style   m_stylePic;    /// 节点样式
};

#endif // CSCENEMODEL_H
