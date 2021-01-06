#ifndef APPGLOBAL_H
#define APPGLOBAL_H

#include <QObject>

class QQuickItem;
class CPlotMap;
class AreaPlanManager;
class QAppGlobal : public QObject
{
    Q_OBJECT
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

    Q_INVOKABLE void init();
private:
    CPlotMap*        m_pPlotMap=nullptr; /// 在map上绘制
    QQuickItem*      m_pOsgItem=nullptr; /// 设置绑定的osg显示
    AreaPlanManager* m_pAreaPlanManager;
};


#endif // APPGLOBAL_H
