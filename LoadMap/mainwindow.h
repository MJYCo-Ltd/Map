#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <list>
#include <QMainWindow>

using namespace std;
namespace Ui {
class MainWindow;
}

class ISceneGraph;
class IMapSceneNode;
class IPersonInfo;
class IPoint;
class ILine;
class IModel;

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

private:
    Ui::MainWindow *ui;
    ISceneGraph*   m_pSceneGraph;
    list<IMapSceneNode*> m_listNode;
    int nTimerID=-1;
    int m_nTimes=0;
    double dL1;
    double dA1;
    double dL2;
    double dA2;
    double dL3;
    double dA3;
    IPersonInfo*  m_pPersonInfo=nullptr;
    IPoint*       m_pPoint = nullptr;
    ILine*        m_pLine = nullptr;
    IModel*       m_pModel=nullptr;
};

#endif // MAINWINDOW_H
