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
    void OpenStereo(bool bOpen)JUDGE_EQUAL_SET_TRUE(bOpen,m_bOpenStereo,m_bStereoChanged)
    bool IsOpenStereo()const{return(m_bOpenStereo);}

    /**
     * @brief 设置立体模式
     * @param 立体模式
     */
    void SetStereoMode(StereoType emStereo)JUDGE_EQUAL_SET_TRUE(emStereo,m_emStereo,m_bStereoChanged);
    StereoType GetStereoMode()const{return(m_emStereo);}

    /**
     * @brief 设置跟踪的节点
     * @param 具体的跟踪节点
     */
    void SetTrackNode(ISceneNode* pNode)JUDGE_EQUAL_SET_TRUE(pNode,m_pTrackNode,m_bTraceNodeChanged)
    ISceneNode* GetTrackNode()const{return(m_pTrackNode);}

    /**
     * @brief 获取视口的屏显控制类
     * @return
     */
    virtual IViewHud* GetHud()=0;

    /**
     * @brief 设置视点位置
     * @param 视点的位置
     * @param 到视点的时间[s]
     */
    virtual void SetViewPoint(const SceneViewPoint&,unsigned int)=0;
    virtual const SceneViewPoint& GetViewPoint()=0;

	/**
	 * @brief 设置追踪视点
	 */
	virtual void SetTrackViewPoint(const SceneViewPoint&) = 0;

    /**
     * @brief 设置home视点
     */
    void SetHomePoint(const SceneViewPoint& stHomtPoint)JUDGE_EQUAL_SET_TRUE(stHomtPoint,m_stHomePoint,m_bHomePointChanged);
    const SceneViewPoint& GetHomePoint()const{return(m_stHomePoint);}

    /**
     * @brief 设置视口大小
     */
    void SetViewPort(const CameraViewPort& stViewPort)JUDGE_EQUAL_SET_TRUE(stViewPort,m_stViewPort,m_bUpdateViewPort)
    const CameraViewPort& GetViewPort()const{return(m_stViewPort);}

    /**
     * @brief 设置投影类型
     */
    void SetProjectType(ProjectType emType)JUDGE_EQUAL_SET_TRUE(emType,m_emProjectType,m_bUpdateProject)
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
     * @brief 控制截图
     */
    virtual void BeginCapture()=0;
    virtual void EndCapture()=0;

    /**
     * @brief 控制拖拽
     */
    virtual void BeginAvoidDrag()=0;
    virtual void EndAvoidDrag()=0;

    /**
     * @brief 设置视口背景颜色
     * @param PortColor
     */
    virtual void SetPortBackColor(const SceneColor& stPortColor)JUDGE_EQUAL_SET_TRUE(stPortColor,m_stBackColor,m_bBackColor);
#ifdef NEED_VR
    /**
     * @brief 将视口的内容显示到VR上
     * @return true 打开VR成功 false 表示打开VR失败
     * @attention 失败原因见Log/ 下的log
     */
    virtual bool ShowOnVR()=0;

    /**
     * @brief 关闭VR
     * @return 如果没有打开VR或者关闭VR成功 返回 true
     */
    virtual bool ShutDownVR()=0;
#endif
protected:
    virtual ~IViewPort(){}
protected:
    StereoType     m_emStereo{ANAGLYPHIC};       /// 立体模式
    bool           m_bOpenStereo{false};         /// 是否开启立体
    bool           m_bStereoChanged{false};      /// 立体模式更改
    bool           m_bHomePointChanged{false};   /// home视点更改
    bool           m_bTraceNodeChanged{false};   /// 跟踪节点更改
    bool           m_bUpdateProject{false};      /// 投影方式更改
    bool           m_bUpdateViewPort{false};     /// 视口更改
    bool           m_bBackColor{false};          /// 窗口背景颜色修改
    ProjectType    m_emProjectType{Perspective}; /// 投影类型
    ISceneNode*    m_pTrackNode{};
    SceneViewPoint m_stHomePoint;
    CameraViewPort m_stViewPort;
    SceneColor     m_stBackColor;                /// 设置背景颜色
};

#endif
