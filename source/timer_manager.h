#pragma once

#include "manager.h"

class TimerManager : public SingleMgr<TimerManager>
{
public:
	TimerManager() = default;
	~TimerManager() = default;

	bool Is_Pause() const
	{
		return this->is_pause;
	}

	void Change_Pause_State()
	{
        is_pause = !is_pause;
	}

private:
	bool is_pause = false;

};
