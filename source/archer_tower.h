#pragma once

#include "tower.h"
#include "resources_manager.h"

class ArcherTower : public Tower
{
public:

	ArcherTower() {
		// 纹理获取
		static SDL_Texture* texture = ResourcesManager::GetInstance()->GetTextureMap().find(ResID::Tex_Archer)->second;

		// 动画
		static const std::vector<int> idx_list_idle_up = { 3, 4 };
        static const std::vector<int> idx_list_idle_down = { 0, 1 };
        static const std::vector<int> idx_list_idle_left = { 6, 7 };
        static const std::vector<int> idx_list_idle_right = { 9, 10 };
		static const std::vector<int> idx_list_fire_up = { 15, 16, 17 };
        static const std::vector<int> idx_list_fire_down = { 12, 13, 14 };
        static const std::vector<int> idx_list_fire_left = { 18, 19, 20 };
        static const std::vector<int> idx_list_fire_right = { 21, 22, 23 };

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

		tower_type = TowerType::Archer;

		fire_speed = 6; // 子弹发射速度
		bullet_type = BulletType::Arrow;
	}
    ~ArcherTower() = default;

};