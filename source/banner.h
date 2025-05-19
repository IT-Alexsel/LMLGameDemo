#pragma once


#include "vector2.h"
#include "timer.h"
#include "config_manager.h"
#include "resources_manager.h"

#include <SDL.h>

class Banner
{
public:
	Banner()
	{
		size_foreground = { 646, 215 };
		size_background = { 1282, 209 };

		timer_display.Set_One_Shotted(true);
		timer_display.Set_Wait_Time(10);
		timer_display.Set_Call_Back(
			[&]() 
			{ 
				is_end_display = true; 
			});

	}
	~Banner() {

	}

	void Update(double delta_time) {
        timer_display.Update(delta_time);

		const ResourcesManager::TextureMap& tex_map = ResourcesManager::GetInstance()->GetTextureMap();
		const ConfigManager* config_mgr = ConfigManager::GetInstance();

		tex_foreground = tex_map.find(config_mgr->is_game_win ? ResID::Tex_UIWinText : ResID::Tex_UILossText)->second;
		tex_background = tex_map.find(ResID::Tex_UIGameOverBar)->second;
	}

	void Render(SDL_Renderer* renderer)
	{
		static SDL_Rect rect_dst;
		rect_dst.x = (int)(pos_center.x - size_background.x / 2);
        rect_dst.y = (int)(pos_center.y - size_background.y / 2);
		rect_dst.w = (int)size_background.x;
        rect_dst.h = (int)size_background.y;
        SDL_RenderCopy(renderer, tex_background, nullptr, &rect_dst);

		rect_dst.x = (int)(pos_center.x - size_foreground.x / 2);
		rect_dst.y = (int)(pos_center.y - size_foreground.y / 2);
		rect_dst.w = (int)size_foreground.x;
		rect_dst.h = (int)size_foreground.y;
        SDL_RenderCopy(renderer, tex_foreground, nullptr, &rect_dst);

	}

	void Set_Center_Position(const Vector2& pos)
	{
        pos_center = pos;
	}

	bool Check_End_Display() const
	{
        return is_end_display;
	}

private:
	Vector2 pos_center;

	Vector2 size_foreground;
    Vector2 size_background;

	SDL_Texture* tex_foreground = nullptr;
	SDL_Texture* tex_background = nullptr;

	Timer timer_display;
	bool is_end_display = false;



};
