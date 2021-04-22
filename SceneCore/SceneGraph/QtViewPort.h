#ifndef QT_VIEWPORT_H
#define QT_VIEWPORT_H
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

class QtViewPort :public QObject,public IViewPort,public IOsgViewPoint
{
    Q_OBJECT
public:
    explicit QtViewPort(IRender* pRender,ISceneGraph* pSceneGraph,ProjectType emProject=Perspective);
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
    const SceneViewPoint& GetViewPoint()const override;

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
protected slots:
    void EyePos(double dX,double dY, double dZ);
    void LookDir(double dX,double dY, double dZ);
protected:
    /// 立体显示模式更改
    void StereoChanged()override{m_bStereoChanged=true;}
    void HomePointChanged()override{m_bHomePointChanged=true;}
    void TrackNodeChanged()override;
    void ViewPortChanged()override;
    void ProjectTypeChanged()override;
    /**
     * @brief 删除从相机
     */
    void RemoveSlave();
protected:
    std::list<osg::ref_ptr<osg::Camera>>           m_listStereoCamera;
    std::list<IViewPortMessageObserver*>           m_pAllOserver;          /// 所有的消息订阅者
    osg::ref_ptr<osgViewer::View>                  m_pView;
    osg::ref_ptr<QtViewPointUpdateCallback>        m_pCameraUpdate;
    osg::ref_ptr<CMyEarthManipulator>              m_p2DEarthManipulator; ///二维地图操作器
    osg::ref_ptr<CMyEarthManipulator>              m_p3DEarthManipulator; ///三维地图操作器
    osg::ref_ptr<osgGA::TrackballManipulator>      m_pSelfManipulator;    /// 没有地图的操作器
    osg::ref_ptr<osgGA::NodeTrackerManipulator>    m_pTrackManipulator;   /// 跟踪操作器
    ViewPointType                                  m_emType=View_Osg;
    ViewPointType                                  m_emPreType;
    double                                         m_dTimes=0.;
    SceneViewPoint m_stViewPoint;

    QtViewHud*     m_pHud=nullptr;                                         /// 屏显根节点
    ISceneGraph*   m_pSceneGraph=nullptr;                                  /// 设置场景
    IRender*       m_pRender;
    bool           m_bStereoChanged=false;
    bool           m_bHomePointChanged=false;     /// home视点更改
    bool           m_bViewPointChanged=false;     /// 视点更改
};

#endif // QT_VIEWPORT_H
