#pragma once
#include <string.h>
#include <fstream>
#include <iostream>
#include <sstream>

#include <SDL.h>
#include <cJSON.h>

#include "manager.h"
#include "map.h"
#include "wave.h"

class ConfigManager : public SingleMgr<ConfigManager> // 配置管理器
{
    friend class SingleMgr<ConfigManager>;

public:
	struct BasicTemplate // 窗口模板
    {
        std::string title = "LML_GameDemo_1";
        int width = 1280;
        int height = 720;
    };
    struct PlayerTemplate // 玩家模板
    {
        double speed = 5;
        double normal_attack_interval = 0.8;
        double normal_attack_damage = 10;
        double skill_interval = 3;
        double skill_damage = 20;
    };
    struct TowerTemplate // 防御塔模板
    {
        double interval[10] = { 1 };
        double damage[10] = { 25 };
        double view_range[10] = { 5 };
        double cost[10] = { 50 };
        double upgrade_cost[9] = { 50 };
    };
    struct EnemyTemplate // 敌人模板
    {
        double hp = 100;
        double speed = 1;
        double damage = 1;
        double reward_ratio = 0.5;
        
        double recover_interval = 10;
        double recover_range = -1;
        double recover_intensity = 25;
    };

public:
    Map map;
    std::vector<Wave> wave_List;

    int level_archer = 0;
    int level_axeman = 0;
    int level_gunner = 0;

    bool is_game_win = true; // 游戏胜利: 房屋生命值
    bool is_game_over = false; // 游戏结束: 所有波次以及所有怪物都死亡

    SDL_Rect rect_tile_map = { 0 }; // 地图区域

    BasicTemplate basic_template;
    
    PlayerTemplate player_template;

    TowerTemplate archer_template;
    TowerTemplate axeman_template;
    TowerTemplate gunner_template;
    
    EnemyTemplate slim_template;
    EnemyTemplate kingSlim_template;
    EnemyTemplate skeleton_template;
    EnemyTemplate goblin_template;
    EnemyTemplate goblinPriest_template;

    const double num_initial_hp = 10;
    const double num_initial_coin = 100;
    const double num_coin_per_prop = 10; 


    bool Load_Level_Config(const std::string& path)
    {
        std::ifstream file(path);
        if (!file.good()) return false;
        std::stringstream str_stream;
        str_stream << file.rdbuf(); // 将文件内容读入字符串流
        file.close();
        cJSON* root = cJSON_Parse(str_stream.str().c_str());
        if (root == nullptr) return false;
        if (root->type != cJSON_Array) 
        {
            cJSON_Delete(root);
            printf("Load_Level_Config]:cJson root is not array\n");
            return false;
        };
        // 解析数据
        cJSON* json_wave = nullptr;
        cJSON_ArrayForEach(json_wave, root)
        {
            if (json_wave->type != cJSON_Object) continue;
            wave_List.emplace_back();
            Wave& wave = wave_List.back();
            cJSON* json_wave_interval = cJSON_GetObjectItem(json_wave, "interval");
            if (json_wave_interval && json_wave_interval->type == cJSON_Number) {
                wave.interval = json_wave_interval->valuedouble;
            }
            cJSON* json_wave_rewards = cJSON_GetObjectItem(json_wave, "rewards");
            if (json_wave_rewards && json_wave_rewards->type == cJSON_Number) {
                wave.rewards = json_wave_rewards->valuedouble;
            }
            // 解析事件数组
            cJSON* json_wave_spawn_list = cJSON_GetObjectItem(json_wave, "spawn_list");
            if (json_wave_spawn_list && json_wave_spawn_list->type == cJSON_Array) {
                cJSON* json_spawn = nullptr;
                cJSON_ArrayForEach(json_spawn, json_wave_spawn_list)
                {
                    if (json_spawn->type != cJSON_Object) continue;
                    wave.spawn_event_list.emplace_back();
                    Wave::SpawnEvent& spawn_event = wave.spawn_event_list.back();
                    cJSON* json_spawn_interval = cJSON_GetObjectItem(json_spawn, "interval");
                    if (json_spawn_interval && json_spawn_interval->type == cJSON_Number)
                    {
                        spawn_event.interval = json_spawn_interval->valuedouble;
                    };
                    cJSON* json_spawn_enemy = cJSON_GetObjectItem(json_spawn, "enemy");
                    if (json_spawn_enemy && json_spawn_enemy->type == cJSON_String)
                    {
                        const std::string enemy_type = json_spawn_enemy->valuestring;
                        if (enemy_type == "Slim") spawn_event.enemy_type = EnemyType::Slim;
                        else if (enemy_type == "KingSlim") spawn_event.enemy_type = EnemyType::KingSlim;
                        else if (enemy_type == "Skeleton") spawn_event.enemy_type = EnemyType::Skeleton;
                        else if (enemy_type == "Goblin") spawn_event.enemy_type = EnemyType::Goblin;
                        else if (enemy_type == "GoblinPriest") spawn_event.enemy_type = EnemyType::GoblinPriest;
                    };
                    cJSON* json_spawn_point = cJSON_GetObjectItem(json_spawn, "point");
                    if (json_spawn_point && json_spawn_point->type == cJSON_Number)
                    {
                        spawn_event.spawn_point = json_spawn_point->valueint;
                    };
                };
                if (wave.spawn_event_list.empty()) 
                {
                    wave_List.pop_back();
                };

            };
            
        };
        cJSON_Delete(root);
        if (wave_List.empty())
        {
            printf("[Load_Level_Config]:wave_List is empty\n");
            return false;
        };
        printf("Load_Level_Config]:Load success\n");
        return true;
    };

    bool Load_Game_Config(const std::string& path)
    {
        printf("[Load_Game_Config]:Loading......\n");
        std::ifstream file(path);
        if (!file.good()) return false;
        std::stringstream str_stream;
        str_stream << file.rdbuf(); // 将文件内容读入字符串流
        file.close();
        cJSON* root = cJSON_Parse(str_stream.str().c_str());
        if (root == nullptr) return false;
        // Basic
        cJSON* json_basic = cJSON_GetObjectItem(root, "basic");
        // Player
        cJSON* json_player = cJSON_GetObjectItem(root, "player");
        // Tower
        cJSON* json_tower = cJSON_GetObjectItem(root, "tower");
        // Enemy
        cJSON* json_enemy = cJSON_GetObjectItem(root, "enemy");

        if (!json_basic || !json_player || !json_tower || !json_enemy ||
            json_basic->type != cJSON_Object || json_player->type != cJSON_Object ||
            json_tower->type != cJSON_Object || json_enemy->type != cJSON_Object
            ) {
            cJSON_Delete(root);
            return false;
        };
        Parse_Basic_Template(basic_template, json_basic);
        
        Parse_Player_Template(player_template, json_player);
        
        Parse_Tower_Template(archer_template, cJSON_GetObjectItem(json_tower, "archer"));
        Parse_Tower_Template(axeman_template, cJSON_GetObjectItem(json_tower, "axeman"));
        Parse_Tower_Template(gunner_template, cJSON_GetObjectItem(json_tower, "gunner"));

        Parse_Enemy_Template(slim_template, cJSON_GetObjectItem(json_enemy, "slim"));
        Parse_Enemy_Template(kingSlim_template, cJSON_GetObjectItem(json_enemy, "king_slim"));
        Parse_Enemy_Template(skeleton_template, cJSON_GetObjectItem(json_enemy, "skeleton"));
        Parse_Enemy_Template(goblin_template, cJSON_GetObjectItem(json_enemy, "goblin"));
        Parse_Enemy_Template(goblinPriest_template, cJSON_GetObjectItem(json_enemy, "goblin_priest"));

        cJSON_Delete(root);
        printf("Load_Game_Config]:Load success\n");
        return true;
    };

protected:
    ConfigManager()=default;
    ~ConfigManager()=default;

private:
    void Parse_Basic_Template(BasicTemplate& btl, cJSON* json_basic)
    {
        if (!json_basic || json_basic->type != cJSON_Object) return;
        cJSON* json_window_width = cJSON_GetObjectItem(json_basic, "window_width");
        cJSON* json_window_height = cJSON_GetObjectItem(json_basic, "window_height");
        cJSON* json_title = cJSON_GetObjectItem(json_basic, "window_title");
        if (json_window_width && json_window_width->type == cJSON_Number)
        {
            btl.width = json_window_width->valueint;
        };
        if (json_window_height && json_window_height->type == cJSON_Number)
        {
            btl.height = json_window_height->valueint;
        };
        if (json_title && json_title->type == cJSON_String)
        {
            btl.title = json_title->valuestring;
        };
        
    };
    void Parse_Player_Template(PlayerTemplate& ptl, cJSON* json_player)
    {
        if (!json_player || json_player->type != cJSON_Object) return;
        cJSON* json_speed = cJSON_GetObjectItem(json_player, "speed");
        if (json_speed && json_speed->type == cJSON_Number)
        {
            ptl.speed = json_speed->valuedouble;
        }
        cJSON* json_normal_attack_interval = cJSON_GetObjectItem(json_player, "normal_attack_interval");
        if (json_normal_attack_interval && json_normal_attack_interval->type == cJSON_Number)
        {
            ptl.normal_attack_interval = json_normal_attack_interval->valuedouble;
        }
        cJSON* json_normal_attack_damage = cJSON_GetObjectItem(json_player, "normal_attack_damage");
        if (json_normal_attack_damage && json_normal_attack_damage->type == cJSON_Number)
        {
            ptl.normal_attack_damage = json_normal_attack_damage->valuedouble;
        }
        cJSON* json_skill_interval = cJSON_GetObjectItem(json_player, "skill_interval");
        if (json_skill_interval && json_skill_interval->type == cJSON_Number)
        {
            ptl.skill_interval = json_skill_interval->valuedouble;
        }
        cJSON* json_skill_damage = cJSON_GetObjectItem(json_player, "skill_damage");
        if (json_skill_damage && json_skill_damage->type == cJSON_Number)
        {
            ptl.skill_damage = json_skill_damage->valuedouble;
        }
    };

    void Parse_Tower_Num_Array(double* array,int max_len, cJSON* json_tower_nums)
    {
        if (!json_tower_nums || json_tower_nums->type != cJSON_Array) return;
        cJSON* json_tower_num = nullptr;
        int index = -1;
        cJSON_ArrayForEach(json_tower_num, json_tower_nums)
        {
            if (json_tower_num->type != cJSON_Number || index >= max_len) continue;
            index++;
            array[index] = json_tower_num->valuedouble;
        };

    };

    void Parse_Tower_Template(TowerTemplate& ttl, cJSON* json_tower)
    {
        if (!json_tower || json_tower->type != cJSON_Object) return;
        cJSON* json_interval = cJSON_GetObjectItem(json_tower, "interval");
        cJSON* json_damage = cJSON_GetObjectItem(json_tower, "damage");
        cJSON* json_view_range = cJSON_GetObjectItem(json_tower, "view_range");
        cJSON* json_cost = cJSON_GetObjectItem(json_tower, "cost");
        cJSON* json_upgrade_cost = cJSON_GetObjectItem(json_tower, "upgrade_cost");
        
        Parse_Tower_Num_Array(ttl.interval, 10, json_interval);
        Parse_Tower_Num_Array(ttl.damage, 10, json_damage);
        Parse_Tower_Num_Array(ttl.view_range, 10, json_view_range);
        Parse_Tower_Num_Array(ttl.cost, 10, json_cost);
        Parse_Tower_Num_Array(ttl.upgrade_cost, 9, json_upgrade_cost);

    };
    void Parse_Enemy_Template(EnemyTemplate& etl, cJSON* json_enemy)
    {
        if (!json_enemy || json_enemy->type != cJSON_Object) return;
        cJSON* json_hp = cJSON_GetObjectItem(json_enemy, "hp");
        cJSON* json_damage = cJSON_GetObjectItem(json_enemy, "damage");
        cJSON* json_reward_ratio = cJSON_GetObjectItem(json_enemy, "reward_ratio");
        cJSON* json_recover_interval = cJSON_GetObjectItem(json_enemy, "recover_interval");
        cJSON* json_recover_range = cJSON_GetObjectItem(json_enemy, "recover_range");
        cJSON* json_recover_intensity = cJSON_GetObjectItem(json_enemy, "recover_intensity");

        if (json_hp && json_hp->type == cJSON_Number)
        {
            etl.hp = json_hp->valuedouble;
        };
        if (json_damage && json_damage->type == cJSON_Number)
        {
            etl.damage = json_damage->valuedouble;
        };
        if (json_reward_ratio && json_reward_ratio->type == cJSON_Number)
        {
            etl.reward_ratio = json_reward_ratio->valuedouble;
        };
        if (json_recover_interval && json_recover_interval->type == cJSON_Number)
        {
            etl.recover_interval = json_recover_interval->valuedouble;
        };
        if (json_recover_range && json_recover_range->type == cJSON_Number)
        {
            etl.recover_range = json_recover_range->valuedouble;
        };
        if (json_recover_intensity && json_recover_intensity->type == cJSON_Number)
        {
            etl.recover_intensity = json_recover_intensity->valuedouble;
        };

    };
};