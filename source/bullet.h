#pragma once
// 子弹管理基类：

#include "vector2.h"
#include "animation.h"
#include "config_manager.h"
#include "enemy.h"


const double PI = 3.141592653589793; // 添加π的定义

class Bullet
{
public:
	Bullet() = default;
	~Bullet() = default;

	void Set_Velocity(const Vector2& velocity) 
	{ 
		this->velocity = velocity; 

		if (can_rotated)
		{
			double randian = std::atan2(velocity.y, velocity.x); // 计算弧度
            angle_anim_rotated = randian * 180 / PI; // 弧度转角度
		}
	}

	void Set_Position(const Vector2& position) { this->position = position; }

	void Set_Damage(double damage) { this->damage = damage; }

	

	const Vector2& Get_Position() const { return position; }
    const Vector2& Get_Velocity() const { return velocity; }
    const Vector2& Get_Size() const { return size; }
    const double Get_Damage() const { return damage; }
    const double Get_Damage_Range() const { return damage_range; }


	void Disable_Collide() { is_collisional = false; }
	bool Can_Collide() const { return is_collisional; }

	void Make_Invalid() { is_valid = false; is_collisional = false; }
    bool Can_Remove() const { return !is_valid; }

	virtual void Update(double delta_time)
	{
		animation.Update(delta_time); // 动画更新
		position += velocity * delta_time; // 位置更新

		// 地图边界检测
		static const SDL_Rect& rect_map = ConfigManager::GetInstance()->rect_tile_map;
		if (position.x - size.x / 2 <= rect_map.x ||
			position.x + size.x / 2 >= rect_map.x + rect_map.w ||
            position.y - size.y / 2 <= rect_map.y ||
            position.y + size.y / 2 >= rect_map.y + rect_map.h)
		{
			is_valid = false;
		}
	}

	virtual void Render(SDL_Renderer* renderer)
	{
		static SDL_Point point;
		point.x = (int)(position.x - size.x / 2);
        point.y = (int)(position.y - size.y / 2);
		animation.Render(renderer, point, angle_anim_rotated); // 动画渲染
	}

	virtual void On_Collision(Enemy* enemy) // 子弹与敌人碰撞时的处理函数
	{
		is_valid = false;
		is_collisional = false;
	}

protected:
	// 飞行属性
	Vector2 position;
	Vector2 velocity;
	Vector2 size;
	// 动画
	Animation animation;
	bool can_rotated = false;
	// 伤害属性
	double damage = 0;
	double damage_range = -1;


private:
	bool is_valid = true; // 子弹是否有效(用来删除)
	bool is_collisional = true; // 是否可碰撞:子弹在与敌人的碰撞帧已经进行了伤害结算，防止重复结算
	
	double angle_anim_rotated = 0;




};