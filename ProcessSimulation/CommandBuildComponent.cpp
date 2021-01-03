#include "CommandBuildComponent.h"

CommandBuildComponent::CommandBuildComponent()
{

}

CommandBuildComponent::~CommandBuildComponent()
{

}

void CommandBuildComponent::execute()
{
    // YTY 查询名为name()的构件，并显示
}

void CommandBuildComponent::undo()
{
    // YTY 查询名为name()的构件，并隐藏
}

void CommandBuildComponent::setExpense(double exp)
{
    _expense = exp;
}

double CommandBuildComponent::expense()
{
    return _expense;
}

void CommandBuildComponent::setHeadcount(double hc)
{
    _headcount = hc;
}

int CommandBuildComponent::headcount()
{
    return _headcount;
}

void CommandBuildComponent::setName(QString name)
{
    _name = name;
}

QString CommandBuildComponent::name()
{
    return _name;
}

void CommandBuildComponent::setDateTime(QDateTime dt)
{
    _datetime = dt;
}

QDateTime CommandBuildComponent::dateTime()
{
    return _datetime;
}
