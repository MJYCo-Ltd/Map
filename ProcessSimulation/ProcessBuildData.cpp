#include "ProcessBuildData.h"
#include "CommandBuildComponent.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>

ProcessBuildData::ProcessBuildData(QObject *parent) : QObject(parent)
{

}

bool ProcessBuildData::init(QString dbFilePath, QString tableName)
{
    _list.clear();
    //打开数据库（获取数据库接口）
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbFilePath);
    if (!db.open())
    {
        qDebug() << "database is error";
        return false;
    }
    QSqlQuery query(db);
    QSqlQueryModel* querymodel = new QSqlQueryModel();
    querymodel->setQuery("SELECT * FROM " + tableName, db);
    while (querymodel->canFetchMore())
    {
        querymodel->fetchMore();
    }
    for (int i = 0; i < querymodel->rowCount(); i++)
    {
        QDateTime dt = QDateTime::fromString(querymodel->data(querymodel->index(i, 0), Qt::DisplayRole).toString(),
            "yyyy/MM/dd");																			//第1列：日期
        QString name = querymodel->data(querymodel->index(i, 1), Qt::DisplayRole).toString();			//第2列：施工内容
        double exp = querymodel->data(querymodel->index(i, 2), Qt::DisplayRole).toDouble();			//第3列：费用合计（万元）
        int hc = querymodel->data(querymodel->index(i, 3), Qt::DisplayRole).toInt();			//第4列：人员

        CommandBuildComponent* cmd = new CommandBuildComponent();
        cmd->setName(name);
        cmd->setDateTime(dt);
        cmd->setHeadcount(hc);
        cmd->setExpense(exp);
        _list.append(cmd);
    }

    ////关闭数据库
    db.close();
    return true;
}

QList<CommandBuildComponent*> ProcessBuildData::genCommandList()
{
    return _list;
}
