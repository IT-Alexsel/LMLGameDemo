#pragma once

#include "timer_manager.h"

#include <functional>


class Timer
{
public:
	Timer() = default;
	~Timer() = default;

	void Set_Wait_Time(double wait_time)
	{
        this->wait_time = wait_time;
	}
	
	void Set_Call_Back(std::function<void()> func)
	{
        call_back_func = func;
	}
	
	void Set_One_Shotted(bool one_shotted)
	{
        this->one_shotted = one_shotted;
	}
	
	void Restart()
	{
		pass_time = 0;
		shotted = false;
	}

	void Update( double delta_time)
	{
		if (TimerManager::GetInstance()->Is_Pause()) return;
		pass_time += delta_time;
		if (pass_time >= wait_time)
		{
			bool can_shot = (!one_shotted) || (one_shotted && !shotted);
			shotted = true;
            if (can_shot)
			{
				call_back_func();
				pass_time -= wait_time;
			}
		};
	}
private:
	double pass_time = 0;
	double wait_time = 0;
	bool shotted = false;
	bool one_shotted = false;
	std::function<void()> call_back_func;

};
