#pragma once

#include "tower.h"
#include "resources_manager.h"


class AxemanTower : public Tower
{
public:
    AxemanTower()
    {   // 纹理获取
        SDL_Texture* texture = ResourcesManager::GetInstance()->GetTextureMap().find(ResID::Tex_Axeman)->second;

        // 动画
        static const std::vector<int> idx_list_idle_up = { 3, 4 };
        static const std::vector<int> idx_list_idle_down = { 0, 1 };
        static const std::vector<int> idx_list_idle_left = { 9, 10 };
        static const std::vector<int> idx_list_idle_right = { 6, 7 };
        static const std::vector<int> idx_list_fire_up = { 15, 16, 17 };
        static const std::vector<int> idx_list_fire_down = { 12, 13, 14 };
        static const std::vector<int> idx_list_fire_left = { 21, 22, 23 };
        static const std::vector<int> idx_list_fire_right = { 18, 19, 20 };

        anim_idle_up.Set_Frame_Data(texture, 3, 8, idx_list_idle_up);
        anim_idle_down.Set_Frame_Data(texture, 3, 8, idx_list_idle_down);
        anim_idle_left.Set_Frame_Data(texture, 3, 8, idx_list_idle_left);
        anim_idle_right.Set_Frame_Data(texture, 3, 8, idx_list_idle_right);
        anim_fire_up.Set_Frame_Data(texture, 3, 8, idx_list_fire_up);
        anim_fire_down.Set_Frame_Data(texture, 3, 8, idx_list_fire_down);
        anim_fire_left.Set_Frame_Data(texture, 3, 8, idx_list_fire_left);
        anim_fire_right.Set_Frame_Data(texture, 3, 8, idx_list_fire_right);

        // 基础数据
        size.x = 48;
        size.y = 48;

        tower_type = TowerType::Axeman;

        fire_speed = 5; // 子弹发射速度
        bullet_type = BulletType::Axe; // 子弹类型

    };
    ~AxemanTower() = default;
};