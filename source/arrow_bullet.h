#pragma once

#include "bullet.h"
#include "resources_manager.h"

class ArrowBullet : public Bullet
{
public:
	ArrowBullet()
	{
		// 动画设置
		static SDL_Texture* tex_arrow = ResourcesManager::GetInstance()->GetTextureMap().find(ResID::Tex_BulletArrow)->second;
		static const std::vector<int> idx_list = { 0, 1 };
		animation.Set_Loop(true);
		animation.Set_Interval(0.1);
		animation.Set_Frame_Data(tex_arrow, 2, 1, idx_list);

		// 基础属性
		can_rotated = true;
		size.x = 48;
        size.y = 48;
	}
	~ArrowBullet() = default;


	 void On_Collision(Enemy* enemy) override
	{
		// 碰撞音效
		static const ResourcesManager::SoundMap& sound_map = ResourcesManager::GetInstance()->GetSoundMap();
		switch (rand() % 3)
		{
            case 0:
                Mix_PlayChannel(-1, sound_map.find(ResID::Sound_ArrowHit_1)->second, 0);
                break;
            case 1:
                Mix_PlayChannel(-1, sound_map.find(ResID::Sound_ArrowHit_2)->second, 0);
                break;
            case 2:
                Mix_PlayChannel(-1, sound_map.find(ResID::Sound_ArrowHit_3)->second, 0);
                break;
		}
        // 碰撞效果
		Bullet::On_Collision(enemy);
	}
};