#pragma once

#include "manager.h"
#include "bullet.h"
#include "bullet_type.h"
#include "arrow_bullet.h"
#include "axe_bullet.h"
#include "shell_bullet.h"

#include <vector>

class BulletManager : public SingleMgr<BulletManager>
{
	friend SingleMgr<BulletManager>;

public:
    typedef std::vector<Bullet*> BulletList;

    void Update(double delta_time)
    {
        // 子弹更新
        for (Bullet* bullet : bullet_List)
            bullet->Update(delta_time);
        // 移除无效子弹
        bullet_List.erase
        (
            std::remove_if
            (bullet_List.begin(), bullet_List.end(), 
                [&](const Bullet* bullet) 
                { 
                    bool del =  bullet->Can_Remove(); 
                    if (del) delete bullet;
                    return del;
                }
            ), bullet_List.end()
        );
    }

    void Render(SDL_Renderer* rander)
    {
        for (Bullet* bullet : bullet_List)
            bullet->Render(rander);
    }

    BulletList& Get_Bullet_List()
    {
        return bullet_List;
    }

    void Fire_Bullet(BulletType type, const Vector2& position, const Vector2& velocity, double damage)
    {
        Bullet* bullet = nullptr;
        switch (type)
        {
        case BulletType::Arrow:
            bullet = new ArrowBullet();
            break;
        case BulletType::Axe:
            bullet = new AxeBullet();
            break;
        case BulletType::Shell:
            bullet = new ShellBullet();
            break;
        }
        bullet->Set_Position(position);
        bullet->Set_Velocity(velocity);
        bullet->Set_Damage(damage);
        bullet_List.push_back(bullet);
        
    }


protected:
    BulletManager() = default;
    ~BulletManager()
    {
        for (Bullet* bullet : bullet_List)
        {
            delete bullet;
        }
    };


private:
    BulletList bullet_List;
};