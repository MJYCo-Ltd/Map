#ifndef CSHOWEARTH_H
#define CSHOWEARTH_H

#include <QMainWindow>

class CShowEarth : public QMainWindow
{
    Q_OBJECT
public:
    explicit CShowEarth(QWidget *parent = nullptr);

signals:

protected slots:
    void OpenFile();

protected:
    void InitUI();
private:
    QToolBar* m_pToolBar;
};

#endif // CSHOWEARTH_H
