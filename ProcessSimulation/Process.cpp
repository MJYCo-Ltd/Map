#include "Process.h"
//#include <QDebug>
Process::Process()
{
	_finished = true;
}

void Process::start()
{
	_finished = false;
}

bool Process::isFinished()
{
	return _finished;
}

void Process::setFinished(bool finished)
{
	_finished = finished;
}
/*
void ProcessDebug::execute(QDateTime dt)
{
	qDebug() << "ProcessDebug execute, QDateTime:" << dt;
}*/