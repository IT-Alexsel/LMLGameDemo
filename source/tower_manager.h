#pragma once

#include "tower.h"
#include "manager.h"
#include "archer_tower.h"
#include "axeman_tower.h"
#include "gunner_tower.h"
#include "config_manager.h"
#include "resources_manager.h"

#include <vector>

class TowerManager : public SingleMgr<TowerManager>
{
    friend class SingleMgr<TowerManager>;
public:

    void Update(double delta_time)
    {
        for (Tower* tower : tower_List)
            tower->Update(delta_time);
    }

    void Render(SDL_Renderer* renderer)
    {
        for (Tower* tower : tower_List)
            tower->Render(renderer);
    }

    double Get_Place_Cost(TowerType type)
    {
        static ConfigManager* configMgr = ConfigManager::GetInstance();
        switch (type)
        {
        case TowerType::Archer:
            return configMgr->archer_template.cost[configMgr->level_archer];
            break;
        case TowerType::Axeman:
            return configMgr->axeman_template.cost[configMgr->level_axeman];
            break;
        case TowerType::Gunner:
            return configMgr->gunner_template.cost[configMgr->level_gunner];
            break;
        }
        return 0;
    }

    double Get_Upgrade_Cost(TowerType type)
    {
        static ConfigManager* configMgr = ConfigManager::GetInstance();
        switch (type)
        {
        case TowerType::Archer:
            return configMgr->level_archer == 9 ? -1 : configMgr->archer_template.upgrade_cost[configMgr->level_archer];
            break;
        case TowerType::Axeman:
            return configMgr->level_axeman == 9 ? -1 : configMgr->axeman_template.upgrade_cost[configMgr->level_axeman];
            break;
        case TowerType::Gunner:
            return configMgr->level_gunner == 9 ? -1 : configMgr->gunner_template.upgrade_cost[configMgr->level_gunner];
            break;
        }
        return 0;
    }

    double Get_Damage_Range(TowerType type) // 显示用
    {
        static ConfigManager* configMgr = ConfigManager::GetInstance();
        switch (type)
        {
        case TowerType::Archer:
            return configMgr->archer_template.view_range[configMgr->level_archer];
            break;
        case TowerType::Axeman:
            return configMgr->axeman_template.view_range[configMgr->level_axeman];
            break;
        case TowerType::Gunner:
            return configMgr->gunner_template.view_range[configMgr->level_gunner];
            break;
        }
        return 0;
    }

    void Place_Tower(TowerType type, const SDL_Point& idx)
    {
        Tower* tower = nullptr;
        switch (type)
        {
        case TowerType::Archer:
            tower = new ArcherTower();
            break;
        case TowerType::Axeman:
            tower = new AxemanTower();
            break;
        case TowerType::Gunner:
            tower = new GunnerTower();
            break;
        default:
            tower = new ArcherTower();
            break;
        }
        // 位置处理
        static Vector2 postion;
        static const SDL_Rect& rect = ConfigManager::GetInstance()->rect_tile_map;
        postion.x = rect.x + idx.x * SIZE_TILE + SIZE_TILE / 2;
        postion.y = rect.y + idx.y * SIZE_TILE + SIZE_TILE / 2;
        tower->Set_Position(postion);
        tower_List.push_back(tower);

        ConfigManager::GetInstance()->map.Place_Tower(idx);

        // 音效
        static const ResourcesManager::SoundMap& soundMap = ResourcesManager::GetInstance()->GetSoundMap();
        Mix_PlayChannel(-1, soundMap.find(ResID::Sound_PlaceTower)->second, 0);
        
    }

    void Upgrade_Tower(TowerType type)
    {
        static ConfigManager* configMgr = ConfigManager::GetInstance();
        switch (type)
        {
        case TowerType::Archer:
            configMgr->level_archer = configMgr->level_archer >= 9 ? 9 : configMgr->level_archer + 1;
            break;
        case TowerType::Axeman:
            configMgr->level_axeman = configMgr->level_axeman >= 9 ? 9 : configMgr->level_axeman + 1;
            break;
        case TowerType::Gunner:
            configMgr->level_gunner = configMgr->level_gunner >= 9 ? 9 : configMgr->level_gunner + 1;
            break;
        }
        // 音效
        static const ResourcesManager::SoundMap& soundMap = ResourcesManager::GetInstance()->GetSoundMap();
        Mix_PlayChannel(-1, soundMap.find(ResID::Sound_TowerLevelUp)->second, 0);
    }

protected:
    TowerManager() = default;
    ~TowerManager() = default;

private:
    std::vector<Tower*> tower_List;



};