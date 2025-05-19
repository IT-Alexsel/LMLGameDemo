#pragma once
// 敌人-史莱姆王

#include "enemy.h"
#include "config_manager.h"
#include "resources_manager.h"

class KingSlimEnemy : public Enemy
{
public:
	KingSlimEnemy()
	{
		// 纹理
		static const ResourcesManager::TextureMap texture_map
			= ResourcesManager::GetInstance()->GetTextureMap();
		static SDL_Texture* texture = texture_map.find(ResID::Tex_KingSlim)->second; // 史莱姆王纹理
		static SDL_Texture* texture_sketch = texture_map.find(ResID::Tex_KingSlimSketch)->second; // 史莱姆王受击纹理
		// 数据
		static ConfigManager::EnemyTemplate& tem = ConfigManager::GetInstance()->kingSlim_template;
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
		static const std::vector<int> idx_list_up = { 18, 19, 20, 21, 22, 23 };
		static const std::vector<int> idx_list_down = { 0, 1, 2, 3, 4, 5 };
		static const std::vector<int> idx_list_left = { 6, 7, 8, 9, 10, 11 };
		static const std::vector<int> idx_list_right = { 12, 13, 14, 15, 16, 17 };

		anim_up.Set_Loop(true);
		anim_up.Set_Interval(0.1);
		anim_up.Set_Frame_Data(texture, 6, 4, idx_list_up);

		anim_down.Set_Loop(true);
		anim_down.Set_Interval(0.1);
		anim_down.Set_Frame_Data(texture, 6, 4, idx_list_down);

		anim_left.Set_Loop(true);
		anim_left.Set_Interval(0.1);
		anim_left.Set_Frame_Data(texture, 6, 4, idx_list_left);

		anim_right.Set_Loop(true);
		anim_right.Set_Interval(0.1);
		anim_right.Set_Frame_Data(texture, 6, 4, idx_list_right);

		anim_up_sketch.Set_Loop(true);
		anim_up_sketch.Set_Interval(0.1);
		anim_up_sketch.Set_Frame_Data(texture_sketch, 6, 4, idx_list_up);

		anim_down_sketch.Set_Loop(true);
		anim_down_sketch.Set_Interval(0.1);
		anim_down_sketch.Set_Frame_Data(texture_sketch, 6, 4, idx_list_down);

		anim_left_sketch.Set_Loop(true);
		anim_left_sketch.Set_Interval(0.1);
		anim_left_sketch.Set_Frame_Data(texture_sketch, 6, 4, idx_list_left);

		anim_right_sketch.Set_Loop(true);
		anim_right_sketch.Set_Interval(0.1);
		anim_right_sketch.Set_Frame_Data(texture_sketch, 6, 4, idx_list_right);


	}
	~KingSlimEnemy() = default;
};