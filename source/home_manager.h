#pragma once


#include "manager.h"
#include "config_manager.h"
#include "resources_manager.h"


class HomeManager : public SingleMgr<HomeManager> // 房屋管理器
{
	friend class SingleMgr<HomeManager>;
public:

	double Get_Hp() const
	{
        return num_hp;
	}

	void Decrease_Hp(double val)
	{
        num_hp -= val;
		if (num_hp <= 0)
			num_hp = 0;
		// 播放房屋受伤音效
		static const ResourcesManager::SoundMap& sm = ResourcesManager::GetInstance()->GetSoundMap();
        Mix_PlayChannel(-1, sm.find(ResID::Sound_HomeHurt)->second, 0);

		if (num_hp <= 0)
		{
			ConfigManager::GetInstance()->is_game_win = false;
			ConfigManager::GetInstance()->is_game_over = true;
		}
            
	}

protected:
	HomeManager()
	{
        num_hp = ConfigManager::GetInstance()->num_initial_hp;
	}
	~HomeManager() = default;
private:
	double num_hp = 0;

};