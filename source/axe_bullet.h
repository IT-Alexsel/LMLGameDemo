#pragma once

#include "bullet.h"
#include "resources_manager.h"

class AxeBullet : public Bullet
{
    public:
        AxeBullet()
        {
            static SDL_Texture* tex_axe = ResourcesManager::GetInstance()->GetTextureMap().find(ResID::Tex_BulletAxe)->second;
            static const std::vector<int> idx_list = { 0, 1, 2, 3, 4, 5, 6, 7};

            animation.Set_Loop(true);
            animation.Set_Interval(0.1);
            animation.Set_Frame_Data(tex_axe, 4, 2, idx_list);

            size.x = 48;
            size.y = 48;


        };
        ~AxeBullet() = default;

        void On_Collision(Enemy* enemy) override
        {
            static const ResourcesManager::SoundMap& sound_map = ResourcesManager::GetInstance()->GetSoundMap();
            switch (rand() % 3)
            {
            case 0:
                Mix_PlayChannel(-1, sound_map.find(ResID::Sound_AxeHit_1)->second, 0);
                break;
            case 1:
                Mix_PlayChannel(-1, sound_map.find(ResID::Sound_AxeHit_2)->second, 0);
                break;
            case 2:
                Mix_PlayChannel(-1, sound_map.find(ResID::Sound_AxeHit_3)->second, 0);
                break;
            }

            enemy->Slow_Down();

            Bullet::On_Collision(enemy);
        }
};
