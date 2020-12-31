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

	// 为测试设置缺省值
	setStartDateTime(QDateTime(QDate(2018, 1, 1), QTime(0, 0, 0)));
	setEndDateTime(QDateTime(QDate(2018, 1, 30), QTime(0, 0, 0)));
	setTimeRatio((2 * 24.0 * 60.0 * 60.0) / 1.0);
	setTimerInterval(200);
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
	foreach(Process* p, _processes)
	{
		if (p != nullptr)
		{
			p->start();
		}
	}
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
		if (p != nullptr)
		{
			if (p->isFinished())
			{
				//removeProcess(p); // 不移除，因为为设置时间时需要跳转到对应的进度，移除则时间跳转不能生效
			}
			else
			{
				p->execute(_curDateTime);
			}
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
	if (dt == _curDateTime)
	{
		return;
	}
	else if( (_state == Simulation_Pause) // 如果是暂停之后跳向较晚的时间，继续即可
		&& (dt > _curDateTime) )
	{
		resume();			
	}
    else // 否则重新开始 ？
	{
		start();
	}
	// 已经开始或恢复模拟
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
