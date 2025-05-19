#pragma once

#include "vector2.h"
#include "timer.h"
#include "tile.h"
#include "resources_manager.h"

#include <SDL.h>

class CoinProp
{
public:
	CoinProp()
	{
		timer_jump.Set_One_Shotted(true);
		timer_jump.Set_Wait_Time(interval_jump);
		timer_jump.Set_Call_Back
		(
			[&]() {
                is_jumping = false;
			}
		);

		timer_live.Set_One_Shotted(true);
		timer_live.Set_Wait_Time(interval_disappear);
		timer_live.Set_Call_Back
		(
			[&]() {
				is_valid = false;
			}
		);

		velocity.x = (rand() % 2 ? 1 : -1) * 2 * SIZE_TILE;
        velocity.y = -3 * SIZE_TILE;

	}
	~CoinProp() = default;

	void Set_Position(const Vector2& pos)
	{
        position = pos;
	}

	const Vector2& Get_Position() const
	{
        return position;
	}

	const Vector2& Get_Size() const
	{
        return size;
	}

	void Make_Invalid()
	{
		is_valid = false;
	}

	bool Can_Remove() const
	{
        return !is_valid;
	}

	void Update(double delta_time)
	{
		timer_jump.Update(delta_time);
        timer_live.Update(delta_time);
		if (is_jumping)
		{
			velocity.y += gravity * delta_time;
		}
		else // 上下浮动
		{
			velocity.x = 0;
			velocity.y = sin(SDL_GetTicks64() / 1000 * 4) * 30;
		}
		position += velocity * delta_time;
	}

	void Render(SDL_Renderer* renderer)
	{
		static SDL_Rect rect = { 0, 0, (int)size.y, (int)size.x };
		static SDL_Texture* texture = ResourcesManager::GetInstance()->GetTextureMap().find(ResID::Tex_Coin)->second;
        
		rect.x = (int)(position.x - size.x / 2);
        rect.y = (int)(position.y - size.y / 2);

		SDL_RenderCopy(renderer, texture, nullptr,&rect);

	}


private:
	Vector2 position;
	Vector2 velocity;

    Timer timer_jump;
    Timer timer_live;

	bool is_valid = true;
	bool is_jumping = true;

	double gravity = 490; // 重力
	double interval_jump = 0.75; // 跳跃间隔
	Vector2 size = { 16, 16 };
	double interval_disappear = 10; // 消失间隔

};