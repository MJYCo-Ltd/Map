#ifndef APPGLOBAL_H
#define APPGLOBAL_H

#include <QObject>

class QQuickItem;
class CPlotMap;
//class ScenarioManager;
class AreaPlanManager;
//Q_DECLARE_METATYPE(ScenarioManager)
//Q_DECLARE_METATYPE(Scenario)
class QAppGlobal : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(ScenarioManager scenarioManager READ scenarioManager NOTIFY scenarioManagerChanged)
public:
    explicit QAppGlobal(QObject *parent = nullptr);
    ~QAppGlobal();

    /**
     * @brief 设置显示的osgquickItem
     * @param pOsgItem
     */
    Q_INVOKABLE void setOsgItem(QQuickItem* pOsgItem);

    /**
     * @brief 标绘
     */
    Q_INVOKABLE void plotALine();

    /**
     * @brief 清空标绘
     */
    Q_INVOKABLE void clearPlot();
private:
    CPlotMap*        m_pPlotMap{}; /// 在map上绘制
    QQuickItem*      m_pOsgItem{}; /// 设置绑定的osg显示
};


#endif // APPGLOBAL_H
