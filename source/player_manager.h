#pragma once

#include "manager.h"
#include "vector2.h"
#include "animation.h"
#include "timer.h"
#include "config_manager.h"
#include "resources_manager.h"
#include "enemy_manager.h"
#include "facing.h"
#include "coin_manager.h"

#include <SDL.h>

class PlayerManager : public SingleMgr<PlayerManager>
{
	friend class SingleMgr<PlayerManager>;
public:
	void On_Input(SDL_Event* e)
	{
		switch (e->type)
		{
			case SDL_KEYDOWN:
				switch (e->key.keysym.sym)
				{
                    case SDLK_w:
                        is_move_up = true;
                        break;
                    case SDLK_s:
                        is_move_down = true;
                        break;
                    case SDLK_a:
                        is_move_left = true;
                        break;
                    case SDLK_d:
                        is_move_right = true;
                        break;
					case SDLK_j:
						On_Release_Flash();
                        break;
					case SDLK_k:
						On_Release_Impact();
						break;
				}
				break;
			case SDL_KEYUP:
				switch (e->key.keysym.sym)
				{
				case SDLK_w:
					is_move_up = false;
					break;
				case SDLK_s:
					is_move_down = false;
					break;
				case SDLK_a:
					is_move_left = false;
					break;
				case SDLK_d:
					is_move_right = false;
					break;
				}
                break;
			default:
				break;
		}
	}

	void Update(double delta_time)
	{
		timer_auto_increase_mp.Update(delta_time);
        timer_release_flash_cd.Update(delta_time);
		// 移动
		Vector2 direction = Vector2(is_move_right - is_move_left, is_move_down - is_move_up).Normalize();
		velocity = direction * speed * SIZE_TILE; // 速度
		
		if (!is_releasing_flash && !is_releasing_impact)
		{
			position += velocity * delta_time;

			const SDL_Rect& rect_map = ConfigManager::GetInstance()->rect_tile_map;
            // 地图边界检测
			if (position.x < rect_map.x) position.x = rect_map.x;
            if (position.x > rect_map.x + rect_map.w) position.x = rect_map.x + rect_map.w;
            if (position.y < rect_map.y) position.y = rect_map.y;
            if (position.y > rect_map.y + rect_map.h) position.y = rect_map.y + rect_map.h;
			// 朝向
			if (velocity.y > 0) facing = Facing::DOWN;
			if (velocity.y < 0) facing = Facing::UP;
			if (velocity.x > 0) facing = Facing::RIGHT;
			if (velocity.x < 0) facing = Facing::LEFT;
			switch (facing)
			{
				case Facing::UP:
					anim_current = &anim_idle_up;
                    break;
                case Facing::DOWN:
                    anim_current = &anim_idle_down;
                    break;
                case Facing::LEFT:
                    anim_current = &anim_idle_left;
                    break;
                case Facing::RIGHT:
                    anim_current = &anim_idle_right;
                    break;
			}
		}
		else
		{
			switch (facing)
			{
			case Facing::UP:
				anim_current = &anim_attack_up;
				break;
			case Facing::DOWN:
				anim_current = &anim_attack_down;
				break;
			case Facing::LEFT:
				anim_current = &anim_attack_left;
				break;
			case Facing::RIGHT:
				anim_current = &anim_attack_right;
				break;
			}
		}
		// 动画更新
		anim_current->Update(delta_time);
		// 技能碰撞检测
		if (is_releasing_flash)
		{
			anim_effect_flash_current->Update(delta_time);
			EnemyManager::EnemyList& enemy_list = EnemyManager::GetInstance()->Get_Enemy_List();
			for (Enemy* enemy : enemy_list)
			{
				if (enemy->Can_Remove()) continue;

				const Vector2& position_enemy = enemy->Get_Position();
				if (position_enemy.x >= rect_hitbox_flash.x &&
					position_enemy.x <= rect_hitbox_flash.x + rect_hitbox_flash.w &&
					position_enemy.y >= rect_hitbox_flash.y &&
					position_enemy.y <= rect_hitbox_flash.y + rect_hitbox_flash.h
					)
				{
					enemy->Decrease_Hp(ConfigManager::GetInstance()->player_template.normal_attack_damage * delta_time);
				}
			}
		}
		if (is_releasing_impact)
		{
            anim_effect_impact_current->Update(delta_time);
			EnemyManager::EnemyList& enemy_list = EnemyManager::GetInstance()->Get_Enemy_List();
			for (Enemy* enemy : enemy_list)
			{
				if (enemy->Can_Remove()) continue;

				const Vector2& position_enemy = enemy->Get_Position();
				if (position_enemy.x >= rect_hitbox_impact.x &&
					position_enemy.x <= rect_hitbox_impact.x + rect_hitbox_impact.w &&
					position_enemy.y >= rect_hitbox_impact.y &&
					position_enemy.y <= rect_hitbox_impact.y + rect_hitbox_impact.h
					)
				{
                    enemy->Decrease_Hp(ConfigManager::GetInstance()->player_template.skill_damage * delta_time);
					enemy->Slow_Down();
				}
			}
		}
		// 金币拾取
        CoinManager::CoinPropList& coin_prop_list = CoinManager::GetInstance()->Get_Coin_Prop_List();
		static const ResourcesManager::SoundMap& sound_map = ResourcesManager::GetInstance()->GetSoundMap();

		for (CoinProp* coin_prop : coin_prop_list)
		{
            if (coin_prop->Can_Remove()) continue;

            const Vector2& position_coin_prop = coin_prop->Get_Position();
			if (position_coin_prop.x >= position.x - size.x / 2 &&
				position_coin_prop.x <= position.x + size.x / 2 &&
				position_coin_prop.y >= position.y - size.y / 2 &&
				position_coin_prop.y <= position.y + size.y / 2)
			{
                coin_prop->Make_Invalid();
				CoinManager::GetInstance()->Increase_Coin(10);

				Mix_PlayChannel(-1, sound_map.find(ResID::Sound_Coin)->second, 0);
			}
		}
	}

	void Render(SDL_Renderer* renderer)
	{
		static SDL_Point point;
		point.x = (int) (position.x - size.x / 2);
        point.y = (int) (position.y - size.y / 2);
		anim_current->Render(renderer, point);

		if (is_releasing_flash)
		{
			point.x = rect_hitbox_flash.x;
            point.y = rect_hitbox_flash.y;
            anim_effect_flash_current->Render(renderer, point);
		}
		if (is_releasing_impact)
		{
            point.x = rect_hitbox_impact.x;
            point.y = rect_hitbox_impact.y;
            anim_effect_impact_current->Render(renderer, point);
		}
	}

	double Get_Current_MP() const
	{
		return mp;
	}

protected:
	PlayerManager()
	{
		// 定时器初始化
		timer_auto_increase_mp.Set_One_Shotted(false);
		timer_auto_increase_mp.Set_Wait_Time(0.1);
		timer_auto_increase_mp.Set_Call_Back
		(
			[&]() 
			{ 
				double interval = ConfigManager::GetInstance()->player_template.skill_interval;
				mp = std::min(mp + 100/(interval / 0.1), 100.0);
			}
		);
		timer_release_flash_cd.Set_One_Shotted(true);
        timer_release_flash_cd.Set_Wait_Time(ConfigManager::GetInstance()->player_template.normal_attack_interval);
		timer_release_flash_cd.Set_Call_Back
		(
			[&]() 
			{
				can_release_flash = true;
			}
		);

        // 动画初始化
		const static ResourcesManager::TextureMap& texture_map = ResourcesManager::GetInstance()->GetTextureMap();
		SDL_Texture* tex_player = texture_map.find(ResID::Tex_Player)->second;
		
		anim_idle_up.Set_Loop(true);anim_idle_up.Set_Interval(0.1);
		anim_idle_up.Set_Frame_Data(tex_player, 4, 8, { 4, 5, 6, 7 });
        anim_idle_down.Set_Loop(true);anim_idle_down.Set_Interval(0.1);
        anim_idle_down.Set_Frame_Data(tex_player, 4, 8, { 0, 1, 2, 3 });
        anim_idle_left.Set_Loop(true);anim_idle_left.Set_Interval(0.1);
        anim_idle_left.Set_Frame_Data(tex_player, 4, 8, { 8, 9, 10, 11 });
        anim_idle_right.Set_Loop(true);anim_idle_right.Set_Interval(0.1);
        anim_idle_right.Set_Frame_Data(tex_player, 4, 8, { 12, 13, 14, 15 });

		anim_attack_up.Set_Loop(true);anim_attack_up.Set_Interval(0.1);
        anim_attack_up.Set_Frame_Data(tex_player, 4, 8, { 20, 21 });
        anim_attack_down.Set_Loop(true);anim_attack_down.Set_Interval(0.1);
        anim_attack_down.Set_Frame_Data(tex_player, 4, 8, { 16, 17 });
        anim_attack_left.Set_Loop(true);anim_attack_left.Set_Interval(0.1);
        anim_attack_left.Set_Frame_Data(tex_player, 4, 8, { 24, 25 });
        anim_attack_right.Set_Loop(true);anim_attack_right.Set_Interval(0.1);
        anim_attack_right.Set_Frame_Data(tex_player, 4, 8, { 28, 29 });

        anim_effect_flash_up.Set_Loop(false);anim_effect_flash_up.Set_Interval(0.1);
		anim_effect_flash_up.Set_Frame_Data(texture_map.find(ResID::Tex_EffectFlash_Up)->second, 5, 1, { 0, 1, 2, 3, 4 });
		anim_effect_flash_up.Set_On_Finished([&]() {is_releasing_flash = false; });
        anim_effect_flash_down.Set_Loop(false);anim_effect_flash_down.Set_Interval(0.1);
        anim_effect_flash_down.Set_Frame_Data(texture_map.find(ResID::Tex_EffectFlash_Down)->second, 5, 1, { 4, 3, 2, 1, 0 });
        anim_effect_flash_down.Set_On_Finished([&]() {is_releasing_flash = false; });
        anim_effect_flash_left.Set_Loop(false);anim_effect_flash_left.Set_Interval(0.1);
        anim_effect_flash_left.Set_Frame_Data(texture_map.find(ResID::Tex_EffectFlash_Left)->second, 1, 5, { 4, 3, 2, 1, 0 });
        anim_effect_flash_left.Set_On_Finished([&]() {is_releasing_flash = false; });
        anim_effect_flash_right.Set_Loop(false);anim_effect_flash_right.Set_Interval(0.1);
        anim_effect_flash_right.Set_Frame_Data(texture_map.find(ResID::Tex_EffectFlash_Right)->second, 1, 5, { 0, 1, 2, 3, 4 });
        anim_effect_flash_right.Set_On_Finished([&]() {is_releasing_flash = false; });

		
		anim_effect_impact_up.Set_Loop(false);anim_effect_impact_up.Set_Interval(0.1);
		anim_effect_impact_up.Set_Frame_Data(texture_map.find(ResID::Tex_EffectImpact_Up)->second, 5, 1, { 0, 1, 2, 3, 4 });
		anim_effect_impact_up.Set_On_Finished([&]() {is_releasing_impact = false; });
		anim_effect_impact_down.Set_Loop(false);anim_effect_impact_down.Set_Interval(0.1);
		anim_effect_impact_down.Set_Frame_Data(texture_map.find(ResID::Tex_EffectImpact_Down)->second, 5, 1, { 4, 3, 2, 1, 0 });
		anim_effect_impact_down.Set_On_Finished([&]() {is_releasing_impact = false; });
		anim_effect_impact_left.Set_Loop(false);anim_effect_impact_left.Set_Interval(0.1);
		anim_effect_impact_left.Set_Frame_Data(texture_map.find(ResID::Tex_EffectImpact_Left)->second, 1, 5, { 4, 3, 2, 1, 0 });
		anim_effect_impact_left.Set_On_Finished([&]() {is_releasing_impact = false; });
		anim_effect_impact_right.Set_Loop(false);anim_effect_impact_right.Set_Interval(0.1);
		anim_effect_impact_right.Set_Frame_Data(texture_map.find(ResID::Tex_EffectImpact_Right)->second, 1, 5, { 0, 1, 2, 3, 4 });
		anim_effect_impact_right.Set_On_Finished([&]() {is_releasing_impact = false; });

		const SDL_Rect& rect_map = ConfigManager::GetInstance()->rect_tile_map;
		position.x = rect_map.x + rect_map.w / 2;
        position.y = rect_map.y + rect_map.h / 2;

		speed = ConfigManager::GetInstance()->player_template.speed;

        size.x = 96;
        size.y = 96;

	}
	~PlayerManager() = default;

private:
	Vector2 size;
	Vector2 position;
	Vector2 velocity; //

	SDL_Rect rect_hitbox_flash = {0}; // 闪电攻击
	SDL_Rect rect_hitbox_impact = {0}; // 冲击波攻击

	double mp = 100;

	double speed = 0;

	bool can_release_flash = true;
	bool is_releasing_flash = false;
	bool is_releasing_impact = false;

	bool is_move_up = false;
    bool is_move_down = false;
    bool is_move_left = false;
    bool is_move_right = false;
	// 默认动画
	Animation anim_idle_up;
    Animation anim_idle_down;
    Animation anim_idle_left;
    Animation anim_idle_right;
	// 攻击动画
    Animation anim_attack_up;
    Animation anim_attack_down;
    Animation anim_attack_left;
    Animation anim_attack_right;
	// 当前动画
    Animation* anim_current = &anim_idle_down;
	// 闪电攻击动画
	Animation anim_effect_flash_up;
    Animation anim_effect_flash_down;
    Animation anim_effect_flash_left;
    Animation anim_effect_flash_right;
	// 当前闪电攻击动画
    Animation* anim_effect_flash_current = nullptr;
	// 冲击波攻击动画
	Animation anim_effect_impact_up;
	Animation anim_effect_impact_down;
	Animation anim_effect_impact_left;
	Animation anim_effect_impact_right;
	// 当前冲击波攻击动画
	Animation* anim_effect_impact_current = nullptr;


	Timer timer_release_flash_cd;
    Timer timer_auto_increase_mp;

	Facing facing = Facing::LEFT;

	void On_Release_Flash() 
	{
		if (!can_release_flash) return;
		if (is_releasing_flash) return;
		can_release_flash = false;
		is_releasing_flash = true;
		// 根据朝向设置攻击范围
		switch (facing)
		{
			case Facing::UP:
				anim_effect_flash_current = &anim_effect_flash_up;
                rect_hitbox_flash.x = (int)(position.x - 68 / 2);
                rect_hitbox_flash.y = (int)(position.y - size.x / 2 - 300);
				rect_hitbox_flash.w = 68; rect_hitbox_flash.h = 300;
				break;
            case Facing::DOWN:
                anim_effect_flash_current = &anim_effect_flash_down;
                rect_hitbox_flash.x = (int)(position.x - 68 / 2);
                rect_hitbox_flash.y = (int)(position.y + size.x / 2);
                rect_hitbox_flash.w = 68; rect_hitbox_flash.h = 300;
                break;
            case Facing::LEFT:
                anim_effect_flash_current = &anim_effect_flash_left;
                rect_hitbox_flash.x = (int)(position.x - size.x / 2 - 300);
                rect_hitbox_flash.y = (int)(position.y - 68 / 2);
                rect_hitbox_flash.w = 300; rect_hitbox_flash.h = 68;
				break;
            case Facing::RIGHT:
                anim_effect_flash_current = &anim_effect_flash_right;
                rect_hitbox_flash.x = (int)(position.x + size.x / 2);
                rect_hitbox_flash.y = (int)(position.y - 68 / 2);
                rect_hitbox_flash.w = 300; rect_hitbox_flash.h = 68;
                break;
		}
        anim_effect_flash_current->Reset();
        timer_release_flash_cd.Restart();

		static const ResourcesManager::SoundMap& sound_map = ResourcesManager::GetInstance()->GetSoundMap();
        Mix_PlayChannel(-1, sound_map.find(ResID::Sound_Flash)->second, 0);
		
	}
	void On_Release_Impact()
	{
        if (mp < 100) return;
        if (is_releasing_impact) return;
        is_releasing_impact = true;
        // 根据朝向设置攻击范围
		switch (facing)
		{
            case Facing::UP:
                anim_effect_impact_current = &anim_effect_impact_up;
                rect_hitbox_impact.x = (int)(position.x - 140 / 2);
                rect_hitbox_impact.y = (int)(position.y - size.x / 2 - 60);
                rect_hitbox_impact.w = 140; rect_hitbox_impact.h = 60;
				break;
            case Facing::DOWN:
                anim_effect_impact_current = &anim_effect_impact_down;
                rect_hitbox_impact.x = (int)(position.x - 140 / 2);
                rect_hitbox_impact.y = (int)(position.y + size.x / 2);
                rect_hitbox_impact.w = 140; rect_hitbox_impact.h = 60;
                break;
            case Facing::LEFT:
                anim_effect_impact_current = &anim_effect_impact_left;
                rect_hitbox_impact.x = (int)(position.x - size.x / 2 - 60);
                rect_hitbox_impact.y = (int)(position.y - 140 / 2);
                rect_hitbox_impact.w = 60; rect_hitbox_impact.h = 140;
                break;
            case Facing::RIGHT:
                anim_effect_impact_current = &anim_effect_impact_right;
                rect_hitbox_impact.x = (int)(position.x + size.x / 2);
                rect_hitbox_impact.y = (int)(position.y - 140 / 2);
                rect_hitbox_impact.w = 60; rect_hitbox_impact.h = 140;
                break;
		}
		mp = 0;
        anim_effect_impact_current->Reset();
        static const ResourcesManager::SoundMap& sound_map = ResourcesManager::GetInstance()->GetSoundMap();
        Mix_PlayChannel(-1, sound_map.find(ResID::Sound_Impact)->second, 0);
	}
};