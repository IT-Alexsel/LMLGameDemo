#pragma once

#include "resources_manager.h"
#include "coin_manager.h"
#include "home_manager.h"
#include "player_manager.h"

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <string>

class StatusBar
{
public:
	StatusBar() = default;
    ~StatusBar() = default;

	void Set_Position(int x, int y)
	{
		position.x = x; 
		position.y = y;
	}

	void Update(SDL_Renderer* renderer)
	{
		static TTF_Font* font = ResourcesManager::GetInstance()->GetFontMap().find(ResID::Font_Main)->second;
		// 清空上一帧的纹理
		if (tex_text_background) SDL_DestroyTexture(tex_text_background);
		tex_text_background = nullptr;
        if (tex_text_foreground) SDL_DestroyTexture(tex_text_foreground);
        tex_text_foreground = nullptr;
		
		std::string str_val = std::to_string((int)CoinManager::GetInstance()->Get_Current_Coin_Num());
		SDL_Surface* surface_text_background = TTF_RenderText_Blended(font, str_val.c_str(), color_text_background);
		SDL_Surface* surface_text_foreground = TTF_RenderText_Blended(font, str_val.c_str(), color_text_foreground);

		width_text = surface_text_background->w;
        height_text = surface_text_background->h;
        tex_text_background = SDL_CreateTextureFromSurface(renderer, surface_text_background);
        tex_text_foreground = SDL_CreateTextureFromSurface(renderer, surface_text_foreground);

		// 释放内存数据
        SDL_FreeSurface(surface_text_background);
        SDL_FreeSurface(surface_text_foreground);

	}

	void Render(SDL_Renderer* renderer)
	{
		// 绘制: 金币、血条、蓝条
		static SDL_Rect rect_dst;
		static const ResourcesManager::TextureMap& texture_map = ResourcesManager::GetInstance()->GetTextureMap();
		
		static SDL_Texture* tex_coin = texture_map.find(ResID::Tex_Coin)->second;
        static SDL_Texture* tex_heart = texture_map.find(ResID::Tex_UIHeart)->second;
		static SDL_Texture* tex_home_avatar = texture_map.find(ResID::Tex_UIHomeAvatar)->second;
        static SDL_Texture* tex_player_avatar = texture_map.find(ResID::Tex_UIPlayerAvatar)->second;
		// 房屋
		rect_dst.x	= position.x;
        rect_dst.y	= position.y;
        rect_dst.w	= 78;
        rect_dst.h	= 78;
        SDL_RenderCopy(renderer, tex_home_avatar, nullptr, &rect_dst);
		// 生命值
		rect_dst.y = position.y;
		rect_dst.w = size_heart;
        rect_dst.h = size_heart;
		for (int i = 0; i < (int)HomeManager::GetInstance()->Get_Hp(); i++)
		{
            rect_dst.x = position.x + 78 + 15 + i * (size_heart + 2); // 78是房屋的宽度，15是房屋和血条之间的间距, 2是血条之间的间距
			SDL_RenderCopy(renderer, tex_heart, nullptr, &rect_dst);
		}
		// 金币
		rect_dst.x = position.x + 78 + 15;
		rect_dst.y = position.y + 78 - 32;
		rect_dst.w = 32;
		rect_dst.h = 32;
		SDL_RenderCopy(renderer, tex_coin, nullptr, &rect_dst);
		// 金币数量
		rect_dst.x += 32 + 10 + offset_shadow.x;
		rect_dst.y = rect_dst.y + (32 - height_text) / 2 + offset_shadow.y; // 居中对齐
        rect_dst.w = width_text;
        rect_dst.h = height_text;
        SDL_RenderCopy(renderer, tex_text_background, nullptr, &rect_dst);

		rect_dst.x -= offset_shadow.x;
		rect_dst.y -= offset_shadow.y;
        SDL_RenderCopy(renderer, tex_text_foreground, nullptr, &rect_dst);

		// 玩家
		rect_dst.x = position.x + (78 - 65) / 2 ;
        rect_dst.y = position.y + 78 + 5;
        rect_dst.w = 65;
        rect_dst.h = 65;
        SDL_RenderCopy(renderer, tex_player_avatar, nullptr, &rect_dst);

		// 蓝条
		rect_dst.x = position.x + 78 + 15;
        rect_dst.y += 10;
        rect_dst.w = width_mp_bar;
        rect_dst.h = height_mp_bar;
		roundedBoxRGBA(renderer, rect_dst.x, rect_dst.y, rect_dst.x + rect_dst.w, rect_dst.y + rect_dst.h, 4,
			color_mp_bar_background.r, color_mp_bar_background.g, color_mp_bar_background.b, color_mp_bar_background.a);

		rect_dst.x += width_border_mp_bar;
        rect_dst.y += width_border_mp_bar;
        rect_dst.w -= width_border_mp_bar * 2;
        rect_dst.h -= width_border_mp_bar * 2;
		double process = PlayerManager::GetInstance()->Get_Current_MP() / 100;

		roundedBoxRGBA(renderer, rect_dst.x, rect_dst.y, rect_dst.x + (int)(rect_dst.w * process), rect_dst.y + rect_dst.h, 2,
			color_mp_bar_foreground.r, color_mp_bar_foreground.g, color_mp_bar_foreground.b, color_mp_bar_foreground.a);
	}

private:
	const int size_heart = 32;
	const int width_mp_bar = 200;
    const int height_mp_bar = 20;
	const int width_border_mp_bar = 4;
	const SDL_Point offset_shadow = { 2, 2 };
	const SDL_Color color_text_background = { 175, 175, 175, 255 };
    const SDL_Color color_text_foreground = { 255, 255, 255, 255 };
    const SDL_Color color_mp_bar_background = { 48, 40, 51, 255 };
    const SDL_Color color_mp_bar_foreground = { 144, 121, 173, 255 };

	SDL_Point position = { 0};
	int width_text = 0, height_text = 0;
	SDL_Texture* tex_text_background = nullptr;
    SDL_Texture* tex_text_foreground = nullptr;


};