#ifndef INTERFACE_VIEW_PORT_HEARDER_H
#define INTERFACE_VIEW_PORT_HEARDER_H
#include <SceneGraph/ViewType.h>
class IViewHud;
class ISceneNode;
class IOsgViewPoint;

/**
 * @brief 视口消息
 */
class IViewPortMessageObserver
{
public:
    virtual ~IViewPortMessageObserver(){}

    /**
     * @brief 视点位置更改
     */
    virtual void EypePos(const ScenePos&){}

    /**
     * @brief 观察点位置
     */
    virtual void LookDir(const ScenePos&){}

    /**
     * @brief 捕获到的照片
     */
    virtual void CaptureImage(const RGBAData*){}
};

/**
 * @brief 视口类
 */
class IViewPort
{
public:
    /// 立体模式
    enum StereoType
    {
        QUAD_BUFFER,
        ANAGLYPHIC,
        HORIZONTAL_SPLIT,
        VERTICAL_SPLIT
    };

    /**
     * @brief 是否开启立体
     */
    void OpenStereo(bool bOpen)JUDGE_EQUAL_CALL_FUNCTION(bOpen,m_bOpenStereo,StereoChanged)
    bool IsOpenStereo()const{return(m_bOpenStereo);}

    /**
     * @brief 设置立体模式
     * @param type
     */
    void SetStereoMode(StereoType emStereo)JUDGE_EQUAL_CALL_FUNCTION(emStereo,m_emStereo,StereoChanged);
    StereoType GetStereoMode()const{return(m_emStereo);}

    /**
     * @brief 设置跟踪的节点
     */
    void SetTrackNode(ISceneNode* pNode)JUDGE_EQUAL_CALL_FUNCTION(pNode,m_pTrackNode,TrackNodeChanged)
    ISceneNode* GetTrackNode()const{return(m_pTrackNode);}

    /**
     * @brief 获取视口的屏显控制类
     * @return
     */
    virtual IViewHud* GetHud()=0;

    /**
     * @brief 设置视点位置
     * @brief 到视点的时间[s]
     */
    virtual void SetViewPoint(const SceneViewPoint&,unsigned int)=0;
    virtual const SceneViewPoint& GetViewPoint()=0;

    /**
     * @brief 设置home视点
     */
    void SetHomePoint(const SceneViewPoint& stHomtPoint)JUDGE_EQUAL_CALL_FUNCTION(stHomtPoint,m_stHomePoint,HomePointChanged);
    const SceneViewPoint& GetHomePoint()const{return(m_stHomePoint);}

    /**
     * @brief 设置视口大小
     */
    void SetViewPort(const CameraViewPort& stViewPort)JUDGE_EQUAL_CALL_FUNCTION(stViewPort,m_stViewPort,ViewPortChanged)
    const CameraViewPort& GetViewPort()const{return(m_stViewPort);}

    /**
     * @brief 设置投影类型
     */
    void SetProjectType(ProjectType emType)JUDGE_EQUAL_CALL_FUNCTION(emType,m_emProjectType,ProjectTypeChanged)
    ProjectType GetProjectType()const{return(m_emProjectType);}

    /**
     * @brief 回到home视点
     */
    virtual void HomeViewPoint()=0;

    /**
     * @brief 订阅消息
     */
    virtual bool SubMessage(IViewPortMessageObserver*)=0;
    virtual bool UnSubMessage(IViewPortMessageObserver*)=0;

    /**
     * @brief 做为OSG的ViewPoint接口
     * @return
     */
    virtual IOsgViewPoint* AsOsgViewPoint(){return(nullptr);}

    /**
     * @brief 开始截图
     */
    virtual void BeginCapture()=0;

    /**
     * @brief 停止截图
     */
    virtual void EndCapture()=0;
protected:
    virtual ~IViewPort(){}

    /// 投影方式更改
    virtual void ProjectTypeChanged()=0;
    /// 视点更改
    virtual void HomePointChanged()=0;
    /// 视口更改
    virtual void ViewPortChanged()=0;
    /// 跟踪节点更改
    virtual void TrackNodeChanged()=0;
    /// 立体状态更改
    virtual void StereoChanged()=0;
protected:
    StereoType     m_emStereo=ANAGLYPHIC;       /// 立体模式
    bool           m_bOpenStereo=false;         /// 是否开启立体
    ProjectType    m_emProjectType=Perspective; /// 投影类型
    ISceneNode*    m_pTrackNode=nullptr;
    SceneViewPoint m_stHomePoint;
    CameraViewPort m_stViewPort;
};

#endif
