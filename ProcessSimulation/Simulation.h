#pragma once
/*************************************************
* Copyright(C)
* File name:    Simulation
* Author:       魏晓亮
* Version:      1.1
* Date:         2020/12/31
* Description:  使用定时器控制模拟的开始、暂停、恢复、停止、跳转
*               通过Process接口调用实现具体的过程模拟
*               支持设置虚拟/现实时间跳动速率比
* History:      2018/07/28 v1.0
*************************************************/
#include "ProcessSimulation_global.h"
#include "Process.h"
#include <QObject>
#include <QDateTime>
#include <QTimer>
#include <QTime>
#include <QList>

// 模拟时间速率/现实时间速率 默认：1天/秒
const float SIMULATION_DEFAULT_TIMERATIO = (24.0 * 60.0 * 60.0) / 1.0; 	// sim : real
// 计时器跳动的时间间隔 默认：30毫秒
const int   SIMULATION_DEFAULT_TIMERINTERVAL = 30;						// unit: msec

enum Simulation_State
{
	Simulation_Run,
	Simulation_Pause,
	Simulation_Stop
};

class PROCESSSIMULATION_EXPORT Simulation : public QObject
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
    void pauseOrResume();                               // switch state (by pushing button) repeatedly
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
