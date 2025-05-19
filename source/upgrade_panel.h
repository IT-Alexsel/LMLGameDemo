#pragma once

#include "panel.h"
#include "resources_manager.h"
#include "tower_manager.h"
#include "config_manager.h"
#include "coin_manager.h"

class UpgradePanel: public Panel
{
public:
    UpgradePanel()
    {
        const ResourcesManager::TextureMap& texture_map = ResourcesManager::GetInstance()->GetTextureMap();
        
        tex_idle = texture_map.find(ResID::Tex_UIUpgradeIdle)->second;
        tex_hovered_top = texture_map.find(ResID::Tex_UIUpgradeHoveredTop)->second;
        tex_hovered_left = texture_map.find(ResID::Tex_UIUpgradeHoveredLeft)->second;
        tex_hovered_right = texture_map.find(ResID::Tex_UIUpgradeHoveredRight)->second;


    }
    ~UpgradePanel() = default;

    void Update(SDL_Renderer* renderer) override
    {
        static TowerManager* tower_manager = TowerManager::GetInstance();

        val_top = (int)tower_manager->Get_Upgrade_Cost(TowerType::Axeman);
        val_left = (int)tower_manager->Get_Upgrade_Cost(TowerType::Archer);
        val_right = (int)tower_manager->Get_Upgrade_Cost(TowerType::Gunner);

        Panel::Update(renderer);
    }

protected:

    void On_Click_Top_Area() override
    {
        static CoinManager* coin_manager = CoinManager::GetInstance();
        if (val_top < 0) return; // 满级
        if (val_top > coin_manager->Get_Current_Coin_Num()) return;
        TowerManager::GetInstance()->Upgrade_Tower(TowerType::Axeman);
        coin_manager->Decrease_Coin(val_top);

    }

    void On_Click_Left_Area() override
    {
        static CoinManager* coin_manager = CoinManager::GetInstance();
        if (val_left < 0) return; // 满级
        if (val_left > coin_manager->Get_Current_Coin_Num()) return;
        TowerManager::GetInstance()->Upgrade_Tower(TowerType::Archer);
        coin_manager->Decrease_Coin(val_left);
    }

    void On_Click_Right_Area() override
    {
        static CoinManager* coin_manager = CoinManager::GetInstance();
        if (val_right < 0) return; // 满级
        if (val_right > coin_manager->Get_Current_Coin_Num()) return;
        TowerManager::GetInstance()->Upgrade_Tower(TowerType::Gunner);
        coin_manager->Decrease_Coin(val_right);
    }

private:

};