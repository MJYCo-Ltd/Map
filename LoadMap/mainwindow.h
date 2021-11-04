#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <list>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class ISceneGraph;
class IMapSceneNode;
class IPersonInfo;
class IPoint;
class IMapLine;
class IModel;
class ISceneNode;
class IMapLayer;
class CPlaceNode;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void SetSecenGraph(ISceneGraph* pSceneGraph);
protected:
    void timerEvent(QTimerEvent *event);
private slots:
    void on_actionchange_triggered();

    void on_action_triggered();

    void on_action_2_triggered();

    void on_action_3_triggered();

    void on_action_4_triggered();

    void on_action_5_triggered();

    void on_action_6_triggered();

    void on_action12_triggered();

    void on_BeginCapture_triggered();

    void on_endCapture_triggered();

    void on_action_OpenVR_triggered();

    void on_action_CloseVR_triggered();

protected:
    void PlotMap();
    void LodPlot();
    void TestGroup();
    void LoadQingxie();
    void TestHud();

private:
    Ui::MainWindow *ui;
    ISceneGraph*   m_pSceneGraph;
    std::list<IMapSceneNode*> m_listNode;
    int nTimerID=-1;
    int m_nTimes=0;
    double dL1;
    double dA1;
    double dL2;
    double dA2;
    double dL3;
    double dA3;
    IMapLayer* m_pLayer=nullptr;
    IPersonInfo*  m_pPersonInfo=nullptr;
    IPoint*       m_pPoint = nullptr;
    IMapLine*        m_pLine = nullptr;
    IModel*       m_pModel=nullptr;
    ISceneNode*    m_pTrackNode=nullptr;
    CPlaceNode*    m_pTestNode{};
    CPlaceNode*    m_pTestNode1{};
};

#endif // MAINWINDOW_H
