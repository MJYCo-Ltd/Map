#include <QToolBar>
#include <QApplication>
#include <QFileDialog>
#include <QTextStream>
#include "ShowEarth.h"

/**
 * @brief 位置信息
 */
struct PosValue
{
    double dX;
    double dY;
    double dValue;
};

/**
 * @brief 覆盖信息
 */
struct CoverInfo
{
    int    nXNum;
    int    nYNum;
    double dMin;
    double dMax;
    std::vector<PosValue>vPosInfo;
};

CShowEarth::CShowEarth(QWidget *parent) : QMainWindow(parent)
{
    InitUI();
}

void CShowEarth::InitUI()
{
    m_pToolBar = new QToolBar;
    addToolBar(m_pToolBar);
    connect(m_pToolBar->addAction(QString::fromUtf8("打开文件")),SIGNAL(triggered()),this,SLOT(OpenFile()));
}

void CShowEarth::OpenFile()
{
    QString sFileName = QFileDialog::getOpenFileName(nullptr,QString::fromUtf8("打开xyz文件"),QApplication::applicationDirPath(),"*.xyz");

    if(!sFileName.isNull())
    {
        QFile file(sFileName);
        if(file.open(QIODevice::ReadOnly))
        {
            QTextStream ssIn(&file);

            double dPreLon(-300),dMinValue(DBL_MAX),dMaxValue(DBL_MIN);

            CoverInfo tmpCoverInfo;
            PosValue tmpPos;

            int nX(1),nY(0);

            while (!ssIn.atEnd())
            {
                ssIn>>tmpPos.dX>>tmpPos.dY>>tmpPos.dValue;
                if(0 == tmpPos.dX)
                {
                    break;
                }

                tmpCoverInfo.vPosInfo.push_back(tmpPos);

                if(tmpPos.dX < dPreLon)
                {
                    ++nX;
                }

                ++nY;

                dPreLon = tmpPos.dX;

                if(tmpPos.dValue < dMinValue)
                {
                    dMinValue = tmpPos.dValue;
                }

                if(tmpPos.dValue > dMaxValue)
                {
                    dMaxValue = tmpPos.dValue;
                }
            }

            tmpCoverInfo.nXNum = nY/nX;
            tmpCoverInfo.nYNum = nX;
            tmpCoverInfo.dMin = dMinValue;
            tmpCoverInfo.dMax = dMaxValue;
            file.close();
        }

    }
}
