#pragma once

#include "manager.h"
#include "timer.h"
#include "config_manager.h"
#include "enemy_manager.h"
#include "coin_manager.h"

class WaveManager : public SingleMgr<WaveManager>
{
    friend class SingleMgr<WaveManager>;
public:
    void Update(double delta_time)
    {
        static ConfigManager* config_mgr = ConfigManager::GetInstance();
        if (config_mgr->is_game_over) return;

        if (!is_wave_started)
            timer_start_wave.Update(delta_time);
        else
            timer_spawn_enemy.Update(delta_time);

        if (is_spawned_last_enemy && EnemyManager::GetInstance()->Check_Cleared())
        {
            CoinManager::GetInstance()->Increase_Coin(config_mgr->wave_List[idx_wave].rewards);
            idx_wave++;
            if (idx_wave >= config_mgr->wave_List.size())
            {
                config_mgr->is_game_win = true;
                config_mgr->is_game_over = true;
            }
            else
            {
                idx_spawn_event = 0;
                is_wave_started = false;
                is_spawned_last_enemy = false;

                const Wave& wave = config_mgr->wave_List[idx_wave];
                timer_start_wave.Set_Wait_Time(wave.interval);
                timer_start_wave.Restart();

            }

        };

    }

protected:
    WaveManager()
    {
        static const std::vector<Wave>& wave_list = ConfigManager::GetInstance()->wave_List;

        timer_start_wave.Set_One_Shotted(true);
        timer_start_wave.Set_Wait_Time(wave_list[0].interval);
        timer_start_wave.Set_Call_Back
        (
            [&]()
            {
                is_wave_started = true;
                timer_spawn_enemy.Set_Wait_Time(wave_list[idx_wave].spawn_event_list[0].interval);
                timer_spawn_enemy.Restart();
            }
        );

        timer_spawn_enemy.Set_One_Shotted(true);
        timer_spawn_enemy.Set_Call_Back
        (
            [&]()
            {
                const std::vector<Wave::SpawnEvent>& spawn_event_list = wave_list[idx_wave].spawn_event_list; // 当前波数中的事件列表
                const Wave::SpawnEvent& spawn_event = spawn_event_list[idx_spawn_event]; // 当前波数中的第几个事件

                EnemyManager::GetInstance()->Spawn_Enemy(spawn_event.enemy_type, spawn_event.spawn_point); // 生成敌人
                idx_spawn_event++; // 当前波数中的第几个事件
                if (idx_spawn_event >= spawn_event_list.size())
                {
                    is_spawned_last_enemy = true;
                    return;
                }
                timer_spawn_enemy.Set_Wait_Time(spawn_event_list[idx_spawn_event].interval);
                timer_spawn_enemy.Restart();

            }
        );
    }
    ~WaveManager() = default;
private:
    int idx_wave = 0; // 当前波数
    int idx_spawn_event = 0; // 当前波数中的第几个事件
    Timer timer_start_wave;
    Timer timer_spawn_enemy;
    bool is_wave_started = false;
    bool is_spawned_last_enemy = false;


};