#ifndef QT_VIEWPORT_H
#define QT_VIEWPORT_H

#include <osgGA/TrackballManipulator>
#include <osgGA/NodeTrackerManipulator>

#include <SceneGraph/IViewPort.h>
#include <Inner/IOsgViewPoint.h>
#include <Plot/Map/IMap.h>
#include <Plot/Map/IMapObserver.h>

class IRender;
class IOsgSceneNode;
class QtViewPointUpdateCallback;
class CMyEarthManipulator;
class QtViewHud;

/// 设置视点状态
enum ViewPointType
{
    View_2D,
    View_3D,
    View_User,
    View_Node
};

class QtViewPort :public IViewPort,public IMapMessageObserver,public IOsgViewPoint
{
public:
    explicit QtViewPort(IRender* pRender,ISceneGraph* pSceneGraph,ProjectType emProject=Perspective);
    ~QtViewPort();

    /**
     * @brief 开启立体显示
     */
    void OpenStereo(bool bOpenStereo)JUDGE_EQUAL_SET_TRUE(bOpenStereo,m_bOpenStereo,m_bStereoChanged)
    /**
     * @brief SetStereoMode
     * @param type
     */
    void SetStereoMode(StereoType type)JUDGE_EQUAL_SET_TRUE(type,m_emStereo,m_bStereoChanged)

    /**
     * @brief 地图类型切换
     * @param emType
     */
    void MapTypeChanged(MapType emType);

    /**
     * @brief 设置跟踪的节点
     */
    bool SetTrackNode(ISceneNode* pTrackNode);

    /**
     * @brief 获取跟踪的节点
     * @return
     */
    ISceneNode* GetTrackNode()const;

    /**
     * @brief 获取视口的屏显控制类
     * @return
     */
    IViewHud* GetHud();

    /**
     * @brief 设置视点位置
     */
    void SetViewPoint(const SceneViewPoint&rViewPoint);

    /**
     * @brief 获取视点位置
     * @return
     */
    const SceneViewPoint& GetViewPoint()const;

    /**
     * @brief 设置窗口大小
     * @param rViewPort
     */
    void SetViewPort(const CameraViewPort& rViewPort);

    /**
     * @brief 设置窗口大小
     * @return
     */
    const CameraViewPort& GetViewPort()const;

    /**
     * @brief 设置投影类型
     */
    void SetProjectType(ProjectType emProject);

    /**
     * @brief 获取投影类型
     * @return 当前的投影类型
     */
    ProjectType GetProjectType()const;

    /**
     * @brief 获取osgView
     * @return
     */
    osgViewer::View* GetOsgView();

    /**
     * @brief 每一帧的回调
     */
    void FrameEvent();
protected:
    void RemoveSlave();
protected:
    std::list<osg::ref_ptr<osg::Camera>>           m_listStereoCamera;
    osg::ref_ptr<osgViewer::View>                  m_pView;
    osg::ref_ptr<QtViewPointUpdateCallback>        m_pCameraUpdate;
    osg::ref_ptr<CMyEarthManipulator>              m_p2DEarthManipulator; ///二维地图操作器
    osg::ref_ptr<CMyEarthManipulator>              m_p3DEarthManipulator; ///三维地图操作器
    osg::ref_ptr<osgGA::CameraManipulator>         m_pSelfManipulator;    /// 没有地图的操作器
    osg::ref_ptr<osgGA::NodeTrackerManipulator>    m_pTrackManipulator;   /// 跟踪操作器
    ViewPointType                                  m_emType=View_User;
    ViewPointType                                  m_emPreType;

    QtViewHud*     m_pHud=nullptr;                                         /// 屏显根节点
    ISceneGraph*   m_pSceneGraph=nullptr;                                  /// 设置场景
    IRender*       m_pRender;
    IOsgSceneNode* m_pTrackNode=nullptr;
    SceneViewPoint m_rViewPoint;
    CameraViewPort m_rViewPort;
    ProjectType    m_emProjectType;
    StereoType     m_emStereo=ANAGLYPHIC;                                 /// 立体模式
    bool           m_bOpenStereo=false;                                   /// 是否开启立体
    bool           m_bStereoChanged=false;
};

#endif // QT_VIEWPORT_H
