#pragma once
// 防御塔

#include "vector2.h"
#include "animation.h"
#include "tower_type.h"
#include "bullet_manager.h"
#include "facing.h"
#include "enemy_manager.h"



class Tower
{
public:
	Tower()
	{
		// 控制开火CD
		timer_fire.Set_One_Shotted(true);
		timer_fire.Set_Call_Back
		(
			[&]()
			{
                can_fire = true;
			}
		);

		// 动画
		anim_idle_up.Set_Loop(true);
		anim_idle_up.Set_Interval(0.2);
		anim_idle_down.Set_Loop(true);
		anim_idle_down.Set_Interval(0.2);
		anim_idle_left.Set_Loop(true);
		anim_idle_left.Set_Interval(0.2);
		anim_idle_right.Set_Loop(true);
		anim_idle_right.Set_Interval(0.2);

		anim_fire_up.Set_Loop(true);
        anim_fire_up.Set_Interval(0.2);
		anim_fire_up.Set_On_Finished
		(
			[&]() {
				Update_Idle_Animation();
			}
		);
		anim_fire_down.Set_Loop(true);
		anim_fire_down.Set_Interval(0.2);
		anim_fire_down.Set_On_Finished
		(
			[&]() {
				Update_Idle_Animation();
			}
		);
		anim_fire_left.Set_Loop(true);
		anim_fire_left.Set_Interval(0.2);
		anim_fire_left.Set_On_Finished
		(
			[&]() {
				Update_Idle_Animation();
			}
		);
		anim_fire_right.Set_Loop(true);
		anim_fire_right.Set_Interval(0.2);
		anim_fire_right.Set_On_Finished
		(
			[&]() {
				Update_Idle_Animation();
			}
		);
	}
	~Tower() = default;

	void Set_Position(const Vector2& position)
	{
		this->position = position;
	}

	const Vector2 Get_Size()const
	{
        return size;
	}

	const Vector2& Get_Position()const
	{
        return position;
	}

	void Update(double delta_time)
	{
		// 定时器更新
		timer_fire.Update(delta_time);
		// 动画更新
		anim_current->Update(delta_time);
		// 开火
		if (can_fire)On_Fire();

	}

	void Render(SDL_Renderer* renderer)
	{
		static SDL_Point point; // 中心点
		point.x = (int)(position.x - size.x / 2);
        point.y = (int)(position.y - size.y / 2);

		anim_current->Render(renderer, point);

	}

protected:
	Vector2 size;
	
	Animation anim_idle_up;
	Animation anim_idle_down;
    Animation anim_idle_left;
    Animation anim_idle_right;

	Animation anim_fire_up;
	Animation anim_fire_down;
	Animation anim_fire_left;
	Animation anim_fire_right;

    TowerType tower_type = TowerType::Archer;

	double fire_speed = 0;

	BulletType bullet_type = BulletType::Arrow;



private:
	Timer timer_fire;
	Vector2 position;
	bool can_fire = true; // 当前是否可以开火
	Facing facing = Facing::RIGHT;
	
	Animation* anim_current = &anim_idle_right;

	void Update_Idle_Animation() // 更新动画
	{
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

	void Update_Fire_Animation() // 更新动画
	{
		switch (facing)
		{
		case Facing::UP:
			anim_current = &anim_fire_up;
			break;				 
		case Facing::DOWN:		 
			anim_current = &anim_fire_down;
			break;				 
		case Facing::LEFT:		 
			anim_current = &anim_fire_left;
			break;				 
		case Facing::RIGHT:		 
			anim_current = &anim_fire_right;
			break;
		}
	}

	Enemy* Find_Target_Enemy() // 寻找目标敌人：优先攻击范围内距离基地最近的敌人
	{
		Enemy* target = nullptr;
		
		double process = -1;
		double view_range = 0;

		static ConfigManager* config_mgr = ConfigManager::GetInstance();
		switch (tower_type)
		{
            case TowerType::Archer:
                view_range = config_mgr->archer_template.view_range[config_mgr->level_archer];
                break;
            case TowerType::Axeman:
                view_range = config_mgr->axeman_template.view_range[config_mgr->level_axeman];
                break;
            case TowerType::Gunner:
                view_range = config_mgr->gunner_template.view_range[config_mgr->level_gunner];
                break;
		}

		const EnemyManager::EnemyList& enemy_list = EnemyManager::GetInstance()->Get_Enemy_List();

		for (Enemy* enemy : enemy_list)
		{
			if (enemy->Can_Remove()) continue;
            double distance = (enemy->Get_Position() - position).Length();
			if ((distance <= view_range * SIZE_TILE) && enemy->Get_Route_Progress() > process)
			{
				target = enemy;
				process = enemy->Get_Route_Progress();
			}
		}
		return target;
	}

	void On_Fire() // 开火
	{
		Enemy* target = Find_Target_Enemy();
		if (!target) return;
		can_fire = false;

		static ConfigManager* config_mgr = ConfigManager::GetInstance();
		static const ResourcesManager::SoundMap& sound_map = ResourcesManager::GetInstance()->GetSoundMap();

		double interval = 0, damage = 0;
		switch (tower_type)
		{
			case TowerType::Archer:
				interval = config_mgr->archer_template.interval[config_mgr->level_archer];
                damage = config_mgr->archer_template.damage[config_mgr->level_archer];
				switch (rand() % 2)
				{
                    case 0:
						Mix_PlayChannel(-1, sound_map.find(ResID::Sound_ArrowFire_1)->second, 0);
						break;
                    case 1:
                        Mix_PlayChannel(-1, sound_map.find(ResID::Sound_ArrowFire_2)->second, 0);
                        break;
				}
            	break;
            case TowerType::Axeman:
                interval = config_mgr->axeman_template.interval[config_mgr->level_axeman];
                damage = config_mgr->axeman_template.damage[config_mgr->level_axeman];
				Mix_PlayChannel(-1, sound_map.find(ResID::Sound_AxeFire)->second, 0);
				break;
            case TowerType::Gunner:
                interval = config_mgr->gunner_template.interval[config_mgr->level_gunner];
                damage = config_mgr->gunner_template.damage[config_mgr->level_gunner];
				Mix_PlayChannel(-1, sound_map.find(ResID::Sound_ShellFire)->second, 0);
                break;
		}

		timer_fire.Set_Wait_Time(interval);
        timer_fire.Restart();
		// 子弹:类型、方向、速度、伤害

		Vector2 direction = target->Get_Position() - position;
        BulletManager::GetInstance()->Fire_Bullet(bullet_type, position, direction.Normalize() * fire_speed * SIZE_TILE, damage);

		bool is_show_x_anim = abs(direction.x) > abs(direction.y);
		if (is_show_x_anim)
			facing = direction.x > 0 ? Facing::RIGHT : Facing::LEFT;
		else
			facing = direction.y > 0 ? Facing::DOWN : Facing::UP;
		// 开火动画更新
		Update_Fire_Animation();
		anim_current->Reset();
	}

};