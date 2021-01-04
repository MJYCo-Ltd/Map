#ifndef PROCESSBUILDDATA_H
#define PROCESSBUILDDATA_H

#include <QObject>

class CommandBuildComponent;
class ProcessBuildData : public QObject
{
    Q_OBJECT
public:
    explicit ProcessBuildData(QObject *parent = nullptr);

    bool init(QString dbFilePath, QString tableName);

    QList<CommandBuildComponent*> genCommandList();
signals:

public slots:

private:
    //QString                         _dbFilePath;
    //QString                         _tableName;
    QList<CommandBuildComponent*>   _list;
};

#endif // PROCESSBUILDDATA_H
