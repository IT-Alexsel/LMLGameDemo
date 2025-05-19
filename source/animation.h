#pragma once
// 动画管理

#include <functional>
#include <vector>

#include <SDL.h>

#include "timer.h"

class Animation
{
public:
	typedef std::function<void()> PlayCallback; // 播放回调函数

	Animation() 
	{
		timer.Set_One_Shotted(false);
		timer.Set_Call_Back(
			[&]() 
			{
				idx_frame++;
				if (idx_frame >= rect_src_list.size()) 
				{
					idx_frame = is_loop ? 0 : rect_src_list.size() - 1;
					if (!is_loop && on_finished) on_finished();

				};
			}
		);
	};
    ~Animation() = default;

	void Reset()
	{
		timer.Restart();
		idx_frame = 0;

	};

	void Set_Frame_Data(SDL_Texture* texture, int num_h, int num_v, const std::vector<int>& idx_list)
	{
		int width_tex, height_tex;
		this->texture = texture;
        SDL_QueryTexture(texture, NULL, NULL, &width_tex, &height_tex);
		width_frame = width_tex / num_h;
        height_frame = height_tex / num_v;
        
		rect_src_list.resize(idx_list.size());
		for (size_t i = 0; i < idx_list.size(); i++)
		{
			int idx = idx_list[i];
			SDL_Rect& rect = rect_src_list[i];
            
			rect.x = (idx % num_h) * width_frame;
            rect.y = (idx / num_h) * height_frame;
            rect.w = width_frame;
			rect.h = height_frame;
		}
	};

	void Set_Loop(bool is_loop)
	{
		this->is_loop = is_loop;
	};

	void Set_Interval(double interval)
	{
        timer.Set_Wait_Time(interval);
	};

	void Set_On_Finished(PlayCallback callback)
	{
		on_finished = callback;
	};

	void Update(double delta_time) {
		timer.Update(delta_time);
	};

	void Render(SDL_Renderer* renderer, const SDL_Point& pos_dst, double angle = 0) const
	{
		static SDL_Rect rect_dst;
		
		rect_dst.x = pos_dst.x;
        rect_dst.y = pos_dst.y;
        rect_dst.w = width_frame;
        rect_dst.h = height_frame;

        SDL_RenderCopyEx(renderer, texture, &rect_src_list[idx_frame], &rect_dst, angle, NULL, SDL_FLIP_NONE); // 绘制当前帧
	};

private:
	Timer timer;
	bool is_loop = true; // 是否循环播放
	size_t idx_frame = 0; // 当前帧索引
    PlayCallback on_finished; // 播放结束回调函数
	SDL_Texture* texture = NULL; 
	std::vector<SDL_Rect> rect_src_list; // 源纹理矩形列表
	int width_frame = 0, height_frame = 0; // 帧宽 帧高
};