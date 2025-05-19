#pragma once

#include <vector>

#include <SDL.h>

#include "manager.h"
#include "enemy.h"
#include "config_manager.h"
#include "home_manager.h"
#include "slim_enemy.h"
#include "king_slim_enemy.h"
#include "skeleton_enemy.h"
#include "goblin_enemy.h"
#include "goblin_priest_enemy.h"
#include "coin_manager.h"
#include "bullet_manager.h"



class EnemyManager : public SingleMgr<EnemyManager>
{
    friend class SingleMgr<EnemyManager>;
public:
    typedef std::vector<Enemy*> EnemyList;

    void Update(double delta_time)
    {
        for (Enemy* enemy : enemyList)
            enemy->Update(delta_time);

        Process_Home_Collision();
        Process_Bullet_Collision();
        Remove_Invalid_Enemy();
    };

    void Render(SDL_Renderer* renderer)
    {
        for (Enemy* enemy : enemyList)
            enemy->Render(renderer);
    }

    void Spawn_Enemy(EnemyType type, int idx_spawn_point)
    {
        if (is_one) return;
        static Vector2 position;
        static const SDL_Rect& rect_tile_map = ConfigManager::GetInstance()->rect_tile_map;
        static const Map::SpawnerRoutePool& spawner_route_pool = ConfigManager::GetInstance()->map.Get_SpawnerRoutePool();
        
        const auto& itor = spawner_route_pool.find(idx_spawn_point);
        if (itor == spawner_route_pool.end()) return; // 没有找到对应的路径
        
        Enemy* enemy = nullptr;

        switch (type)
        {
            case EnemyType::Slim:
                enemy = new SlimEnemy();
                break;
            case EnemyType::KingSlim:
                enemy = new KingSlimEnemy();
                break;
            case EnemyType::Skeleton:
                enemy = new SkeletonEnemy();
                break;
            case EnemyType::Goblin:
                enemy = new GoblinEnemy();
                break;
            case EnemyType::GoblinPriest:
                enemy = new GoblinPriestEnemy();
                break;
            default:
                enemy = new SlimEnemy();
                break;
        }
        // 技能处理
        enemy->Set_On_Skill_Release
        (
            [&](Enemy* enemy_src)
            {
                double recover_radius = enemy_src->Get_Recover_Radius();
                if (recover_radius <= 0.0) return;
                const Vector2& position_src = enemy_src->Get_Position();
                for (Enemy* enemy_dst : enemyList)
                {
                    if (enemy_dst->Can_Remove()) continue;
                    const Vector2& position_dst = enemy_dst->Get_Position();
                    double distance = (position_dst - position_src).Length();
                    if (distance <= recover_radius) enemy_dst->Increase_Hp(enemy_src->Get_Recover_Interval());
                }
            }
        );
        // 位置处理
        const Route::IdxList& idx_list = itor->second.Get_Idx_List();
        position.x = rect_tile_map.x + idx_list[0].x * SIZE_TILE + SIZE_TILE / 2;
        position.y = rect_tile_map.y + idx_list[0].y * SIZE_TILE + SIZE_TILE / 2;
        enemy->Set_Position(position);
        enemy->Set_Route(&itor->second); // 设置路径
        enemyList.push_back(enemy);
        //is_one = true;
    }

    bool Check_Cleared()
    {
        return enemyList.empty();
    }
    
    EnemyList& Get_Enemy_List()
    {
        return enemyList;
    }

protected:
    EnemyManager() = default;
    ~EnemyManager()
    {
        for (Enemy* enemy : enemyList)
            delete enemy;
    };

private:
    EnemyList enemyList;

    bool is_one = false;

    void Process_Home_Collision() // 处理与基地的碰撞
    {
        static const SDL_Point idx_home = ConfigManager::GetInstance()->map.Get_Idx_Home();
        static const SDL_Rect& rect_tile_map = ConfigManager::GetInstance()->rect_tile_map;

        static const Vector2 position_home_tile =
        {
            (double)rect_tile_map.x + idx_home.x * SIZE_TILE,
            (double)rect_tile_map.y + idx_home.y * SIZE_TILE
        };
        for (Enemy* enemy : enemyList)
        {
            if (enemy->Can_Remove()) continue;
            const Vector2& position = enemy->Get_Position();
            // 碰撞检测
            if (position.x >= position_home_tile.x &&
                position.y >= position_home_tile.y &&
                position.x <= position_home_tile.x + SIZE_TILE &&
                position.y <= position_home_tile.y + SIZE_TILE)
            {
                enemy->Make_Invalid();
                HomeManager::GetInstance()->Decrease_Hp(enemy->Damage());
            }
        }
    }

    void Process_Bullet_Collision() // 处理与子弹的碰撞
    {
        static BulletManager::BulletList& bullet_List = BulletManager::GetInstance()->Get_Bullet_List();

       for (Enemy* enemy: enemyList)
        {
           if (enemy->Can_Remove()) continue;
            const Vector2& pos_enemy = enemy->Get_Position();
            const Vector2& size_enemy = enemy->Get_Size();
    
            for (Bullet* bullet: bullet_List)
            {
                if (!bullet->Can_Collide()) continue;

                const Vector2& pos_bullet = bullet->Get_Position();
                const Vector2& size_bullet = bullet->Get_Size();
                if (pos_bullet.x >= pos_enemy.x - size_enemy.x / 2 && 
                    pos_bullet.x <= pos_enemy.x + size_enemy.x / 2 &&
                    pos_bullet.y >= pos_enemy.y - size_enemy.y / 2 &&
                    pos_bullet.y <= pos_enemy.y + size_enemy.y / 2)
                {
                    double damage = bullet->Get_Damage();
                    double damage_range = bullet->Get_Damage_Range();
                    if (damage_range < 0)
                    {
                        enemy->Decrease_Hp(damage);
                        if (enemy->Can_Remove()) Try_Spawn_Coin_Prop(pos_enemy, enemy->Get_Reward_Ratio());
                    }
                    else
                    { 
                        for (Enemy* enemy_dst : enemyList)
                        {
                            const Vector2& pos_enemy_dst = enemy_dst->Get_Position();
                            double distance = (pos_enemy_dst - pos_enemy).Length();
                            if (distance <= damage_range)
                            {
                                enemy_dst->Decrease_Hp(damage);
                                if (enemy_dst->Can_Remove()) Try_Spawn_Coin_Prop(pos_enemy_dst, enemy_dst->Get_Reward_Ratio());
                            }
                        }
                    }
                    bullet->On_Collision(enemy);
                }
            }
        }
        
    }

    void Remove_Invalid_Enemy() // 移除无效的敌人
    {
        enemyList.erase(std::remove_if(
            enemyList.begin(), enemyList.end(),
            [](const Enemy* enemy) {
               bool del = enemy->Can_Remove();
               if (del) delete enemy;
               return del;
            }), enemyList.end());
    }

    void Try_Spawn_Coin_Prop(const Vector2& position, double ratio)
    {
        if ((double)(rand() % 100) <= ratio * 100)
        {
            CoinManager::GetInstance()->Spawn_Coin_Prop(position);
        }

    }
};