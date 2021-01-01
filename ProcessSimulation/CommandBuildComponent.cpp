#include "CommandBuildComponent.h"

CommandBuildComponent::CommandBuildComponent()
{

}

CommandBuildComponent::~CommandBuildComponent()
{

}

void CommandBuildComponent::execute()
{

}

void CommandBuildComponent::undo()
{

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
