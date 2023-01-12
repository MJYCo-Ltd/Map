#ifndef QT_VIEWPORT_H
#define QT_VIEWPORT_H

#ifdef NEED_VR
#include <openvr/openvr.h>
#endif

#include <QObject>
#include <osgGA/TrackballManipulator>
#include <osgGA/NodeTrackerManipulator>

#include <SceneGraph/IViewPort.h>
#include <SceneGraph/ISceneGraph.h>
#include <Inner/IOsgViewPoint.h>

class IRender;
class IOsgSceneNode;
class QtViewPointUpdateCallback;
class CMyEarthManipulator;
class QtViewHud;
struct CaptureImageCallback;

class QtViewPort :public QObject,public IViewPort,public IOsgViewPoint
{
    Q_OBJECT
public:
    explicit QtViewPort(IRender* pRender, ISceneGraph* pSceneGraph);
    ~QtViewPort();

    /**
     * @brief 获取视口的屏显控制类
     * @return
     */
    IViewHud* GetHud() override;

    /**
     * @brief 地图类型切换
     * @param emType
     */
    void ViewPointTypeChanged(ViewPointType emType) override;

    /**
     * @brief 设置视点位置
     */
    void SetViewPoint(const SceneViewPoint&rViewPoint,unsigned int unTimes) override;
    const SceneViewPoint& GetViewPoint() override;

	virtual void SetTrackViewPoint(const SceneViewPoint&) override;
    /**
     * @brief 获取osgView
     * @return
     */
    osgViewer::View* GetOsgView()override;

    /**
     * @brief 每一帧的回调
     */
    void FrameEvent();

    /**
     * @brief 回到Home视点
     */
    void HomeViewPoint()override;

    /**
     * @brief 订阅消息
     * @return
     */
    bool SubMessage(IViewPortMessageObserver*)override;

    /**
     * @brief 订阅消息
     * @return
     */
    bool UnSubMessage(IViewPortMessageObserver*)override;

    /**
     * @brief 做为OSG的接口
     * @return
     */
    IOsgViewPoint* AsOsgViewPoint()override{return(this);}

    /**
     * @brief 开始截图
     */
    void BeginCapture() override;

    /**
     * @brief 停止截图
     */
    void EndCapture() override;

    /**
     * @brief 锁定视角
     */
    void SetLockView(bool)override;

#ifdef NEED_VR
    /**
     * @brief 将视口的内容显示到VR上
     * @return true 打开VR成功 false 表示打开VR失败
     * @attention 失败原因见Log/ 下的log
     */
    virtual bool ShowOnVR() override;

    /**
     * @brief 关闭VR
     * @return 如果没有打开VR或者关闭VR成功 返回 true
     */
    virtual bool ShutDownVR() override;
protected:
    std::string GetTrackedDeviceString(vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError *peError = NULL );
#endif
protected slots:
    void EyePos(double dX,double dY, double dZ);
    void LookDir(double dX,double dY, double dZ);
    void CaptureImage(int nWidht, int nHeight,int nSize,QByteArray info);
protected:
    /**
     * @brief 删除从相机
     */
    void RemoveSlave();

    /**
     * @brief 更新当前视点
     */
    void UpdateViewPoint(CMyEarthManipulator* pEarthManipulator);
    void UpdateViewPoint(osgGA::OrbitManipulator* pManipulator);
protected:
    std::list<osg::ref_ptr<osg::Camera>>           m_listStereoCamera;
    std::list<IViewPortMessageObserver*>           m_pAllOserver;          /// 所有的消息订阅者
    osg::ref_ptr<osgViewer::View>                  m_pView;
    osg::ref_ptr<CaptureImageCallback>             m_pPostDrawBack;       /// 绘制完成的回调
    osg::ref_ptr<CMyEarthManipulator>              m_p2DEarthManipulator; ///二维地图操作器
    osg::ref_ptr<CMyEarthManipulator>              m_p3DEarthManipulator; ///三维地图操作器
    osg::ref_ptr<osgGA::TrackballManipulator>      m_pSelfManipulator;    /// 没有地图的操作器
    osg::ref_ptr<osgGA::NodeTrackerManipulator>    m_pTrackManipulator;   /// 跟踪操作器
    ViewPointType                                  m_emType{View_Osg};
    ViewPointType                                  m_emPreType{View_Osg};
    double                                         m_dTimes{0.};
    bool           m_bViewPointChanged{false};
    bool           m_bViewTypeChanged{false};
    bool           m_bRemoveCapture{false};
    SceneViewPoint m_stViewPoint;
	SceneViewPoint m_stTrackViewPoint;									///跟踪视角
    QtViewHud*     m_pHud{};                                         /// 屏显根节点
    ISceneGraph*   m_pSceneGraph{};                                  /// 设置场景
    IRender*       m_pRender{};
#ifdef NEED_VR
    vr::IVRSystem *m_pHMD{};
    bool           m_bVRStatusChanged{false};
    bool           m_bOpenVR{false};
    unsigned int   m_nVrWidth{};
    unsigned int   m_nVrHeight{};
    std::list<osg::ref_ptr<osg::Camera>>           m_listVRCamera;    /// vr眼镜对应的相机
#endif
};

#endif // QT_VIEWPORT_H
