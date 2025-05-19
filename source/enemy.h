#pragma once

#include <functional>

#include "vector2.h"
#include "timer.h"
#include "animation.h"
#include "route.h"
#include "config_manager.h"

class Enemy
{
public:
    typedef std::function<void(Enemy* enemy)> SkillCallBack;

    Enemy() 
    {
        timer_skill.Set_One_Shotted(false);
        timer_skill.Set_Call_Back([&](){on_skill_release(this);});

        timer_sketch.Set_One_Shotted(true);
        timer_sketch.Set_Wait_Time(0.1);
        timer_sketch.Set_Call_Back([&](){is_show_sketch = false;});

        timer_restore_speed.Set_One_Shotted(true);
        timer_restore_speed.Set_Call_Back([&](){speed = max_speed;});
    };
    ~Enemy() = default;

    void Update(double delta_time) 
    {
        timer_skill.Update(delta_time);
        timer_sketch.Update(delta_time);
        timer_restore_speed.Update(delta_time);

        // 移动 方向 速度
        Vector2 move_distance = velocity * delta_time;  // 计算这一帧移动距离
        Vector2 target_distance = position_target - position; // 目标距离
        position += move_distance < target_distance ? move_distance : target_distance; // 更新位置

        if (target_distance.Approx_Zero())
        {
            idx_target++;
            Refresh_Position_Target();
            
            direction = (position_target - position).Normalize();
        }
        
        velocity.x = direction.x * speed * SIZE_TILE;
        velocity.y = direction.y * speed * SIZE_TILE;

        // 动画
        bool is_show_x_anim = abs(velocity.x) >= abs(velocity.y); // 是否显示X轴动画
        
        if (is_show_sketch) 
        {
            if (is_show_x_anim)
                anim_current = velocity.x > 0 ? &anim_right_sketch : &anim_left_sketch;
            else
                anim_current = velocity.y > 0 ? &anim_down_sketch : &anim_up_sketch;
        }
        else
        {
            if (is_show_x_anim)
                anim_current = velocity.x > 0 ? &anim_right : &anim_left;
            else
                anim_current = velocity.y > 0 ? &anim_down : &anim_up;
        }

        anim_current->Update(delta_time);
    };

    void Render(SDL_Renderer* renderer)
    {   
        static SDL_Rect rect;
        static SDL_Point point;
        static const int offset_y = 2;
        static const Vector2 size_hp_bar = { 40, 8 };
        static const SDL_Color color_border = { 116, 185, 124, 255 };
        static const SDL_Color color_content = { 226, 255, 194, 255 };
        
        point.x = (int)(position.x - size.x / 2);
        point.y = (int)(position.y - size.y / 2);
        anim_current->Render(renderer, point);

        if (hp < max_hp)
        {
            rect.x = (int)(position.x - size_hp_bar.x / 2);
            rect.y = (int)(position.y - size.y / 2 - size_hp_bar.y - offset_y);
            rect.w = (int)(size_hp_bar.x * hp / max_hp);
            rect.h = (int)size_hp_bar.y;
            SDL_SetRenderDrawColor(renderer, color_content.r, color_content.g, color_content.b, color_content.a);
            SDL_RenderFillRect(renderer, &rect); // 填充内容

            rect.w = (int)size_hp_bar.x;
            SDL_SetRenderDrawColor(renderer, color_border.r, color_border.g, color_border.b, color_border.a);
            SDL_RenderDrawRect(renderer, &rect); // 绘制边框
        }
    }

    void Set_On_Skill_Release(SkillCallBack func)
    {
        on_skill_release = func;
    }

    void Increase_Hp(double val)
    {
        hp += val;
        if (hp > max_hp) hp = max_hp;
    }

    void Decrease_Hp(double val) 
    {
        hp -= val;
        if (hp <= 0) 
        { 
            hp = 0;
            is_live = false;
        };
        is_show_sketch = true;
        timer_sketch.Restart();
    }

    void Slow_Down()
    {
        speed = max_speed - 0.5;
        timer_restore_speed.Set_Wait_Time(1);
        timer_restore_speed.Restart();
    }

    void Set_Position(const Vector2& pos)
    {
        position = pos;
    }

    void Set_Route(const Route* route)
    {
        this->route = route;
        Refresh_Position_Target();
    }

    void Make_Invalid()
    {
        is_live = false;
    }

    double Get_HP() const
    {
        return hp;
    }

    const Vector2& Get_Position() const
    {
        return position;
    }

    const Vector2& Get_Size() const
    {
        return size;
    }

    const Vector2& Get_Velocity() const
    {
        return velocity;
    }

    const double Damage() const
    {
        return damage;
    }

    double Get_Reward_Ratio() const
    {
        return reward_ratio;
    }

    double Get_Recover_Radius() const
    {
        return recover_range * SIZE_TILE;
    }

    double Get_Recover_Intensity() const
    {
        return recover_intensity;
    }
    double Get_Recover_Interval() const
    {
        return recover_interval;
    }

    bool Can_Remove() const
    {
        return !is_live;
    }

    double Get_Route_Progress() const // 路线进度
    {
        if (route->Get_Idx_List().size() == 1)
            return 1;
        return (double)idx_target / (route->Get_Idx_List().size() - 1);
    }

protected:
    Vector2 size;
    Timer timer_skill;

    Animation anim_up;
    Animation anim_down;
    Animation anim_left;
    Animation anim_right;

    Animation anim_up_sketch;
    Animation anim_down_sketch;
    Animation anim_left_sketch;
    Animation anim_right_sketch;

    double hp = 0;
    double max_hp = 0;
    double speed = 0;
    double max_speed = 0;
    double damage = 0;
    double reward_ratio = 0;
    double recover_interval = 0;
    double recover_range = 0;
    double recover_intensity = 0;

private:
    Vector2 position;
    Vector2 velocity;
    Vector2 direction;

    bool is_live = true;

    Timer timer_sketch; // 受击闪烁计时器
    bool is_show_sketch = false;

    Animation* anim_current = nullptr; // 当前动画

    SkillCallBack on_skill_release; // 技能释放回调

    Timer timer_restore_speed; // 恢复速度计时器

    const Route* route = nullptr; // 路线
    int idx_target = 0; // 下一个目标点索引
    Vector2 position_target; // 下一个位置

    void Refresh_Position_Target()
    {
        const Route::IdxList& idx_list = route->Get_Idx_List();
        if (idx_target < idx_list.size())
        {
            const SDL_Point& point = idx_list[idx_target];

            static const SDL_Rect& rect_tile_map = ConfigManager::GetInstance()->rect_tile_map;
            position_target.x = rect_tile_map.x + point.x * SIZE_TILE + SIZE_TILE / 2; // 计算下一个目标点（目标格中心点）
            position_target.y = rect_tile_map.y + point.y * SIZE_TILE + SIZE_TILE / 2;
        }
    }
};