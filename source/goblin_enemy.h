#pragma once
// 敌人-哥布林

#include "enemy.h"
#include "config_manager.h"
#include "resources_manager.h"

class GoblinEnemy : public Enemy
{
public:
	GoblinEnemy()
	{
		// 纹理
		static const ResourcesManager::TextureMap texture_map
			= ResourcesManager::GetInstance()->GetTextureMap();
		static SDL_Texture* texture = texture_map.find(ResID::Tex_Goblin)->second; // 哥布林纹理
		static SDL_Texture* texture_sketch = texture_map.find(ResID::Tex_GoblinSketch)->second; // 哥布林受击纹理
		// 数据
		static ConfigManager::EnemyTemplate& tem = ConfigManager::GetInstance()->goblin_template;
		max_hp = tem.hp;
		hp = max_hp;
		max_speed = tem.speed;
		speed = tem.speed;
		damage = tem.damage;
		reward_ratio = tem.reward_ratio;
		recover_interval = tem.recover_interval;
		recover_range = tem.recover_range;
		recover_intensity = tem.recover_intensity;

		size.x = 48;
		size.y = 48;

		// 动画
		static const std::vector<int> idx_list_up = { 5, 6, 7, 8, 9 };
		static const std::vector<int> idx_list_down = { 0, 1, 2, 3, 4 };
		static const std::vector<int> idx_list_left = { 15, 16, 17, 18, 19 };
		static const std::vector<int> idx_list_right = { 10, 11, 12, 13, 14 };

		anim_up.Set_Loop(true);
		anim_up.Set_Interval(0.1);
		anim_up.Set_Frame_Data(texture, 5, 4, idx_list_up);

		anim_down.Set_Loop(true);
		anim_down.Set_Interval(0.1);
		anim_down.Set_Frame_Data(texture, 5, 4, idx_list_down);

		anim_left.Set_Loop(true);
		anim_left.Set_Interval(0.1);
		anim_left.Set_Frame_Data(texture, 5, 4, idx_list_left);

		anim_right.Set_Loop(true);
		anim_right.Set_Interval(0.1);
		anim_right.Set_Frame_Data(texture, 5, 4, idx_list_right);

		anim_up_sketch.Set_Loop(true);
		anim_up_sketch.Set_Interval(0.1);
		anim_up_sketch.Set_Frame_Data(texture_sketch, 5, 4, idx_list_up);

		anim_down_sketch.Set_Loop(true);
		anim_down_sketch.Set_Interval(0.1);
		anim_down_sketch.Set_Frame_Data(texture_sketch, 5, 4, idx_list_down);

		anim_left_sketch.Set_Loop(true);
		anim_left_sketch.Set_Interval(0.1);
		anim_left_sketch.Set_Frame_Data(texture_sketch, 5, 4, idx_list_left);

		anim_right_sketch.Set_Loop(true);
		anim_right_sketch.Set_Interval(0.1);
		anim_right_sketch.Set_Frame_Data(texture_sketch, 5, 4, idx_list_right);


	}
	~GoblinEnemy() = default;
};