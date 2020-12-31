#pragma once
/*************************************************
* Copyright(C)
* File name:    Simulation
* Author:       where2go
* Version:      1.0
* Date:         2018/07/28
* Description:  模拟。启动或停止定时器来控制模拟的进行。
* History:
*************************************************/
#include <QObject>
#include <QDateTime>
#include <QTimer>
#include <QTime>
#include <QList>
#include "Process.h"

//模拟的时间比率 天/秒
const float SIMULATION_DEFAULT_TIMERATIO = (24.0 * 60.0 * 60.0) / 1.0; 	// sim : real
const int   SIMULATION_DEFAULT_TIMERINTERVAL = 100;						// unit: msec

enum Simulation_State
{
	Simulation_Run,
	Simulation_Pause,
	Simulation_Stop
};

class Simulation : public QObject
{
	Q_OBJECT
public:
	Simulation();
	~Simulation();

	void addProcess(Process* process);
	void removeProcess(Process* process);

	void setTimeRatio(float ratio);						// sim : real
	void setStartDateTime(const QDateTime & dt);		
	const QDateTime startDateTime();
	void setEndDateTime(const QDateTime & dt);			
	const QDateTime endDateTime();
	const QDateTime currentDateTime();						
	void setTimerInterval(int msec);					// set period of tick 
	int timerInterval() const; 							// period of tick, unit : msec
	Simulation_State state() { return _state; }
public slots:
	void start();
	void pause();
	void resume();
	void stop();
	void pauseOrResume();
	void goTo(QDateTime& dt);
private slots:
	void executeProcess();
private:
	QList<Process*>		_processes;	
	float				_timeRatio;						// sim : real 
	QDateTime			_startDateTime;					// sim date time
	QDateTime			_endDateTime;					// sim date time
	QDateTime			_curDateTime;					// sim date time	
	QTimer*				_timer;		
	Simulation_State	_state;
};