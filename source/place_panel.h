#pragma once

#include "panel.h"
#include "tower_manager.h"
#include "resources_manager.h"
#include "coin_manager.h"

#include <SDL2_gfxPrimitives.h>

class PlacePanel: public Panel
{
public:
	PlacePanel()
	{
		const ResourcesManager::TextureMap& texture_map = ResourcesManager::GetInstance()->GetTextureMap();

		tex_idle = texture_map.find(ResID::Tex_UIPlaceIdle)->second;
		tex_hovered_top = texture_map.find(ResID::Tex_UIPlaceHoveredTop)->second;
        tex_hovered_left = texture_map.find(ResID::Tex_UIPlaceHoveredLeft)->second;
        tex_hovered_right = texture_map.find(ResID::Tex_UIPlaceHoveredRight)->second;

	}
	~PlacePanel() = default;

	void Update(SDL_Renderer* renderer) override
	{
		static TowerManager* tower_manager = TowerManager::GetInstance();
		
		val_top = (int)tower_manager->Get_Place_Cost(TowerType::Axeman);
        val_left = (int)tower_manager->Get_Place_Cost(TowerType::Archer);
        val_right = (int)tower_manager->Get_Place_Cost(TowerType::Gunner);
		
		reg_top = (int)tower_manager->Get_Damage_Range(TowerType::Axeman) * SIZE_TILE;
        reg_left = (int)tower_manager->Get_Damage_Range(TowerType::Archer) * SIZE_TILE;
        reg_right = (int)tower_manager->Get_Damage_Range(TowerType::Gunner) * SIZE_TILE;

		Panel::Update(renderer);
	}
	
	void Render(SDL_Renderer* renderer) override
	{
		if (!visible) return;

		int reg = 0;
		switch (hovered_target)
		{
			case HoveredTarget::Top:
                reg = reg_top;
                break;
            case HoveredTarget::Left:
                reg = reg_left;
                break;
            case HoveredTarget::Right:
                reg = reg_right;
                break;
		}
		if (reg > 0)
		{
			filledCircleRGBA(renderer, center_pos.x, center_pos.y, reg,
				color_region_content.r, color_region_content.g, color_region_content.b, color_region_content.a);
            aacircleRGBA(renderer, center_pos.x, center_pos.y, reg,
                color_region_frame.r, color_region_frame.g, color_region_frame.b, color_region_frame.a);
		}

		Panel::Render(renderer);
	}

protected:

	void On_Click_Top_Area() override
	{
		static CoinManager* coin_manager = CoinManager::GetInstance();

		if (val_top > coin_manager->Get_Current_Coin_Num()) return;

		TowerManager::GetInstance()->Place_Tower(TowerType::Axeman, idx_tile_selected);
        coin_manager->Decrease_Coin(val_top);

	}

	void On_Click_Left_Area() override
	{
		static CoinManager* coin_manager = CoinManager::GetInstance();

		if (val_left > coin_manager->Get_Current_Coin_Num()) return;

		TowerManager::GetInstance()->Place_Tower(TowerType::Archer, idx_tile_selected);
		coin_manager->Decrease_Coin(val_left);

	}

	void On_Click_Right_Area() override
	{
		static CoinManager* coin_manager = CoinManager::GetInstance();

		if (val_right > coin_manager->Get_Current_Coin_Num()) return;

		TowerManager::GetInstance()->Place_Tower(TowerType::Gunner, idx_tile_selected);
		coin_manager->Decrease_Coin(val_right);

	}

private:
	const SDL_Color color_region_frame = { 30, 80, 162, 175 };
	const SDL_Color color_region_content = { 0, 149, 217, 175 };

	int reg_top = 0, reg_left = 0, reg_right = 0;


};
