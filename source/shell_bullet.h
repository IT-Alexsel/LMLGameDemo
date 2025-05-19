#pragma once

#include "bullet.h"
#include "resources_manager.h"

class ShellBullet : public Bullet
{
public:
    ShellBullet()
    {
        static SDL_Texture* tex_shell = ResourcesManager::GetInstance()->GetTextureMap().find(ResID::Tex_BulletShell)->second;
        static SDL_Texture* tex_explode = ResourcesManager::GetInstance()->GetTextureMap().find(ResID::Tex_EffectExplode)->second;

        static const std::vector<int> idx_list = { 0,1 };
        static const std::vector<int> idx_explode_list = { 0, 1, 2, 3, 4 };


        animation.Set_Loop(true);
        animation.Set_Interval(0.1);
        animation.Set_Frame_Data(tex_shell, 2, 1, idx_list);
    
        animation_explode.Set_Loop(false);
        animation_explode.Set_Interval(0.1);
        animation_explode.Set_Frame_Data(tex_explode, 5, 1, idx_explode_list);
        animation_explode.Set_On_Finished
        (
            [&]() 
            {
                Make_Invalid();
            }
        );
        
        damage_range = 48 * 2;
        
        size.x = 48;
        size.y = 48;

    }

    ~ShellBullet() = default;

    void Update(double delta_time) override
    {
        if (Can_Collide())
        {
            Bullet::Update(delta_time);
            return;
        }
        animation_explode.Update(delta_time);

    }
    void Render(SDL_Renderer* renderer) override
    {
        if (Can_Collide())
        {
            Bullet::Render(renderer);
            return;
        }
        static SDL_Point point;
        point.x = (int)(position.x - damage_range / 2);
        point.y = (int)(position.y - damage_range / 2);
        animation_explode.Render(renderer, point);
    }

    void On_Collision(Enemy* enemy) override
    {
        // 音效
        Mix_PlayChannel(-1, ResourcesManager::GetInstance()->GetSoundMap().find(ResID::Sound_ShellHit)->second, 0);
        Disable_Collide();
    }


private:
    Animation animation_explode;

};