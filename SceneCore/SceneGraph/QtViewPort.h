#ifndef QT_VIEWPORT_H
#define QT_VIEWPORT_H

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

class QtViewPort :public IViewPort,public IOsgViewPoint
{
public:
    explicit QtViewPort(IRender* pRender,ISceneGraph* pSceneGraph,ProjectType emProject=Perspective);
    ~QtViewPort();

    /**
     * @brief 获取视口的屏显控制类
     * @return
     */
    IViewHud* GetHud();

    /**
     * @brief 地图类型切换
     * @param emType
     */
    void ViewPointTypeChanged(ViewPointType emType);

    /**
     * @brief 设置视点位置
     */
    void SetViewPoint(const SceneViewPoint&rViewPoint,unsigned int unTimes);
    const SceneViewPoint& GetViewPoint()const;

    /**
     * @brief 获取osgView
     * @return
     */
    osgViewer::View* GetOsgView();

    /**
     * @brief 每一帧的回调
     */
    void FrameEvent();

    /**
     * @brief 回到Home视点
     */
    void HomeViewPoint(){m_bBackHome=true;}
protected:
    /// 立体显示模式更改
    void StereoChanged(){m_bStereoChanged=true;}
    void HomePointChanged(){m_bHomePointChanged=true;}
    void TrackNodeChanged();
    void ViewPortChanged();
    void ProjectTypeChanged();
    /**
     * @brief 删除从相机
     */
    void RemoveSlave();
protected:
    std::list<osg::ref_ptr<osg::Camera>>           m_listStereoCamera;
    osg::ref_ptr<osgViewer::View>                  m_pView;
    osg::ref_ptr<QtViewPointUpdateCallback>        m_pCameraUpdate;
    osg::ref_ptr<CMyEarthManipulator>              m_p2DEarthManipulator; ///二维地图操作器
    osg::ref_ptr<CMyEarthManipulator>              m_p3DEarthManipulator; ///三维地图操作器
    osg::ref_ptr<osgGA::TrackballManipulator>      m_pSelfManipulator;    /// 没有地图的操作器
    osg::ref_ptr<osgGA::NodeTrackerManipulator>    m_pTrackManipulator;   /// 跟踪操作器
    ViewPointType                                  m_emType=View_User;
    ViewPointType                                  m_emPreType;
    double                                         m_dTimes=0.;
    SceneViewPoint m_stViewPoint;

    QtViewHud*     m_pHud=nullptr;                                         /// 屏显根节点
    ISceneGraph*   m_pSceneGraph=nullptr;                                  /// 设置场景
    IRender*       m_pRender;
    bool           m_bStereoChanged=false;
    bool           m_bHomePointChanged=false;     /// home视点更改
    bool           m_bViewPointChanged=false;     /// 视点更改
    bool           m_bBackHome=false;
};

#endif // QT_VIEWPORT_H
