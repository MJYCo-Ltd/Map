#include "Simulation.h"
#include <QDebug>

Simulation::Simulation()
{
	_timeRatio		= SIMULATION_DEFAULT_TIMERATIO;						// sim : real 
	_startDateTime	= QDateTime::currentDateTime();						// sim date time
	_endDateTime	= QDateTime::currentDateTime();						// sim date time
	_curDateTime	= QDateTime::currentDateTime();						// sim date time	
	_timer			= new QTimer;										
	_timer->setInterval(SIMULATION_DEFAULT_TIMERINTERVAL);
	_state			= Simulation_Stop;
	connect(_timer, SIGNAL(timeout()), this, SLOT(executeProcess()));
}

Simulation::~Simulation()
{
}

void Simulation::setTimeRatio(float ratio)
{
	_timeRatio = ratio;
}

const QDateTime Simulation::startDateTime()
{
	return _startDateTime;
}

void Simulation::setStartDateTime(const QDateTime & dt)
{
	_startDateTime = dt;
}

const QDateTime Simulation::endDateTime()
{
	return _startDateTime;
}

void Simulation::setEndDateTime(const QDateTime & dt)
{
	_endDateTime = dt;
}

const QDateTime  Simulation::currentDateTime()
{
	return _curDateTime;
}

void Simulation::setTimerInterval(int msec)
{
	_timer->setInterval(msec);
}

int Simulation::timerInterval() const
{
	return _timer->interval();
}

void Simulation::start()
{
	_state = Simulation_Run;
	_timer->start();
    _curDateTime = _startDateTime;
}

void Simulation::pause()
{
	if (_state == Simulation_Run)
	{
		_state = Simulation_Pause;
		_timer->stop();
	}
}

void Simulation::resume()
{
	if (_state == Simulation_Pause)
	{
		_state = Simulation_Run;
		_timer->start();
	}
}

void Simulation::stop()
{
	if (_state == Simulation_Run || _state == Simulation_Pause)
	{
		_state = Simulation_Stop;
		_timer->stop();
	}
}

void Simulation::pauseOrResume()
{
	if (_state == Simulation_Run)
	{
		pause();
	}
	else if (_state == Simulation_Pause)
	{
		resume();
	}
}

void Simulation::executeProcess()
{
	foreach(Process* p, _processes)
	{
        if (p == nullptr)
            continue;
        if ( _curDateTime > p->startDateTime() && _curDateTime < p->endDateTime() )
        {
            p->goTo(_curDateTime);
        }
	}
	
    if (_state == Simulation_Run)
    {
        _curDateTime = _curDateTime.addMSecs(_timeRatio * _timer->interval());
        if (_curDateTime > _endDateTime)
        {
            stop();
        }
    }
}

void Simulation::goTo(QDateTime& dt)
{
    if (dt == _curDateTime || dt < _startDateTime || dt > _endDateTime)
	{
		return;
	}
    if (_state == Simulation_Stop)
    {
        start();
    }
    else if(_state == Simulation_Pause)
    {
        resume();
    }
    //else if( (_state == Simulation_Pause) // 如果是暂停之后跳向较晚的时间，继续即可
    //	&& (dt > _curDateTime) )
    //{
    //	resume();
    //}
    //else // 否则重新开始 ？
    //{
    //	start();
    //}

	_curDateTime = dt;	// 跳转当前时间
	executeProcess();	// 执行当前时间的事件
    pause();			// 跳转后暂停在当前时间
}

void Simulation::addProcess(Process* process)
{
	if (_processes.indexOf(process) < 0)
		_processes.append(process);
}

void Simulation::removeProcess(Process* process)
{
	_processes.removeAt(_processes.indexOf(process));
}
