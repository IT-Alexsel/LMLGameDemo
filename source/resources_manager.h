#pragma once

#include <unordered_map>
#include <string>

#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "manager.h"

enum class ResID // 资源纹理ID
{
	Tex_Tileset,

	Tex_Player,
	// 防御塔
	Tex_Archer,
	Tex_Axeman,
	Tex_Gunner,
	
	// 怪物
	Tex_Slim,
	Tex_KingSlim,
	Tex_Skeleton,
	Tex_Goblin,
	Tex_GoblinPriest,
	// 怪物对应受击
	Tex_SlimSketch, 
	Tex_KingSlimSketch,
	Tex_SkeletonSketch,
	Tex_GoblinSketch,
	Tex_GoblinPriestSketch,

	// 子弹
	Tex_BulletArrow,
	Tex_BulletAxe,
	Tex_BulletShell,

	// 金币
	Tex_Coin,
	// 房屋
	Tex_Home,

	// 玩家攻击特效
	Tex_EffectFlash_Up,
	Tex_EffectFlash_Down,
	Tex_EffectFlash_Left,
	Tex_EffectFlash_Right,
	Tex_EffectImpact_Up,
	Tex_EffectImpact_Down,
	Tex_EffectImpact_Left,
	Tex_EffectImpact_Right,
	Tex_EffectExplode,

	// UI相关
	Tex_UISelectCursor,
	Tex_UIPlaceIdle,
	Tex_UIPlaceHoveredTop,
	Tex_UIPlaceHoveredLeft,
	Tex_UIPlaceHoveredRight,
	Tex_UIUpgradeIdle,
	Tex_UIUpgradeHoveredTop,
	Tex_UIUpgradeHoveredLeft,
	Tex_UIUpgradeHoveredRight,
	Tex_UIHomeAvatar,
	Tex_UIPlayerAvatar,
	Tex_UIHeart,
	Tex_UICoin,
	Tex_UIGameOverBar,
	Tex_UIWinText,
	Tex_UILossText,

	// 音效
	Sound_ArrowFire_1,
	Sound_ArrowFire_2,
	Sound_AxeFire,
	Sound_ShellFire,
	Sound_ArrowHit_1,
	Sound_ArrowHit_2,
	Sound_ArrowHit_3,
	Sound_AxeHit_1,
	Sound_AxeHit_2,
	Sound_AxeHit_3,
	Sound_ShellHit,

	Sound_Flash,
	Sound_Impact,

	Sound_Coin,
	Sound_HomeHurt,
	Sound_PlaceTower,
	Sound_TowerLevelUp,

	Sound_Win,
	Sound_Loss,

	Music_BGM,
	// 字体
	Font_Main
};

class ResourcesManager : public SingleMgr<ResourcesManager>
{
friend SingleMgr<ResourcesManager>;

public:
	typedef std::unordered_map<ResID, TTF_Font*> FontMap; // 字体资源映射
    typedef std::unordered_map<ResID, Mix_Chunk*> SoundMap; // 音效资源映射
    typedef std::unordered_map<ResID, Mix_Music*> MusicMap; // 音乐资源映射
    typedef std::unordered_map<ResID, SDL_Texture*> TextureMap; // 纹理资源映射

public:
	bool Load_From_File(SDL_Renderer* renderer)
	{
		texture_Map[ResID::Tex_Tileset] = IMG_LoadTexture(renderer, "res/tileset.png"); // 加载瓦片纹理

		texture_Map[ResID::Tex_Player] = IMG_LoadTexture(renderer, "res/player.png"); // 加载玩家纹理

        texture_Map[ResID::Tex_Archer] = IMG_LoadTexture(renderer, "res/tower_archer.png"); // 加载弓箭手纹理
        texture_Map[ResID::Tex_Axeman] = IMG_LoadTexture(renderer, "res/tower_axeman.png"); // 加载斧头
        texture_Map[ResID::Tex_Gunner] = IMG_LoadTexture(renderer, "res/tower_gunner.png"); // 加载枪手
        
		texture_Map[ResID::Tex_Slim] = IMG_LoadTexture(renderer, "res/enemy_slime.png"); // 加载史莱姆
        texture_Map[ResID::Tex_KingSlim] = IMG_LoadTexture(renderer, "res/enemy_king_slime.png"); // 加载史莱姆王
        texture_Map[ResID::Tex_Skeleton] = IMG_LoadTexture(renderer, "res/enemy_skeleton.png"); // 加载骷髅兵
        texture_Map[ResID::Tex_Goblin] = IMG_LoadTexture(renderer, "res/enemy_goblin.png"); // 加载哥布林
        texture_Map[ResID::Tex_GoblinPriest] = IMG_LoadTexture(renderer, "res/enemy_goblin_priest.png"); // 加载哥布林牧师
        
		texture_Map[ResID::Tex_SlimSketch] = IMG_LoadTexture(renderer, "res/enemy_slime_sketch.png"); // 加载史莱姆受击
        texture_Map[ResID::Tex_KingSlimSketch] = IMG_LoadTexture(renderer, "res/enemy_king_slime_sketch.png"); // 加载史莱姆王受击
        texture_Map[ResID::Tex_SkeletonSketch] = IMG_LoadTexture(renderer, "res/enemy_skeleton_sketch.png"); // 加载骷髅兵受击
        texture_Map[ResID::Tex_GoblinSketch] = IMG_LoadTexture(renderer, "res/enemy_goblin_sketch.png"); // 加载哥布林受击
        texture_Map[ResID::Tex_GoblinPriestSketch] = IMG_LoadTexture(renderer, "res/enemy_goblin_priest_sketch.png"); // 加载哥布林牧师受击

        texture_Map[ResID::Tex_BulletArrow] = IMG_LoadTexture(renderer, "res/bullet_arrow.png"); // 加载弓箭子弹
        texture_Map[ResID::Tex_BulletAxe] = IMG_LoadTexture(renderer, "res/bullet_axe.png"); // 加载斧
        texture_Map[ResID::Tex_BulletShell] = IMG_LoadTexture(renderer, "res/bullet_shell.png"); // 加载炮弹

        texture_Map[ResID::Tex_Coin] = IMG_LoadTexture(renderer, "res/coin.png"); // 加载金币纹理
        texture_Map[ResID::Tex_Home] = IMG_LoadTexture(renderer, "res/home.png"); // 加载房屋纹理

        texture_Map[ResID::Tex_EffectFlash_Up] = IMG_LoadTexture(renderer, "res/effect_flash_up.png"); // 加载向上特效
        texture_Map[ResID::Tex_EffectFlash_Down] = IMG_LoadTexture(renderer, "res/effect_flash_down.png"); // 加载向下特效
        texture_Map[ResID::Tex_EffectFlash_Left] = IMG_LoadTexture(renderer, "res/effect_flash_left.png"); // 加载向左特效
        texture_Map[ResID::Tex_EffectFlash_Right] = IMG_LoadTexture(renderer, "res/effect_flash_right.png"); // 加载向右特效
        
		texture_Map[ResID::Tex_EffectImpact_Up] = IMG_LoadTexture(renderer, "res/effect_impact_up.png"); // 加载向上特效
        texture_Map[ResID::Tex_EffectImpact_Down] = IMG_LoadTexture(renderer, "res/effect_impact_down.png"); // 加载向下特效
        texture_Map[ResID::Tex_EffectImpact_Left] = IMG_LoadTexture(renderer, "res/effect_impact_left.png"); // 加载向左特效
        texture_Map[ResID::Tex_EffectImpact_Right] = IMG_LoadTexture(renderer, "res/effect_impact_right.png"); // 加载向右特效
		texture_Map[ResID::Tex_EffectExplode] = IMG_LoadTexture(renderer, "res/effect_explode.png"); // 加载爆炸特效


        texture_Map[ResID::Tex_UISelectCursor] = IMG_LoadTexture(renderer, "res/ui_select_cursor.png"); // 加载选择光标
        texture_Map[ResID::Tex_UIPlaceIdle] = IMG_LoadTexture(renderer, "res/ui_place_idle.png"); // 加载放置光标
        texture_Map[ResID::Tex_UIPlaceHoveredTop] = IMG_LoadTexture(renderer, "res/ui_place_hovered_top.png"); // 加载放置光标_上
        texture_Map[ResID::Tex_UIPlaceHoveredLeft] = IMG_LoadTexture(renderer, "res/ui_place_hovered_left.png"); // 加载放置光标_左
        texture_Map[ResID::Tex_UIPlaceHoveredRight] = IMG_LoadTexture(renderer, "res/ui_place_hovered_right.png"); // 加载放置光标_右
        texture_Map[ResID::Tex_UIUpgradeIdle] = IMG_LoadTexture(renderer, "res/ui_upgrade_idle.png"); // 加载升级光标
        texture_Map[ResID::Tex_UIUpgradeHoveredTop] = IMG_LoadTexture(renderer, "res/ui_upgrade_hovered_top.png"); // 加载升级光标_上
        texture_Map[ResID::Tex_UIUpgradeHoveredLeft] = IMG_LoadTexture(renderer, "res/ui_upgrade_hovered_left.png"); // 加载升级光标_左
        texture_Map[ResID::Tex_UIUpgradeHoveredRight] = IMG_LoadTexture(renderer, "res/ui_upgrade_hovered_right.png"); // 加载升级光标_右
        texture_Map[ResID::Tex_UIHomeAvatar] = IMG_LoadTexture(renderer, "res/ui_home_avatar.png"); // 加载房屋头像
        texture_Map[ResID::Tex_UIPlayerAvatar] = IMG_LoadTexture(renderer, "res/ui_player_avatar.png"); // 加载玩家头像
        texture_Map[ResID::Tex_UIHeart] = IMG_LoadTexture(renderer, "res/ui_heart.png"); // 加载生命条
        texture_Map[ResID::Tex_UICoin] = IMG_LoadTexture(renderer, "res/ui_coin.png"); // 加载金币
        texture_Map[ResID::Tex_UIGameOverBar] = IMG_LoadTexture(renderer, "res/ui_game_over_bar.png"); // 加载游戏结束条( todo lml: libpng warning: iCCP: known incorrect sRGB profile)
        texture_Map[ResID::Tex_UIWinText] = IMG_LoadTexture(renderer, "res/ui_win_text.png"); // 加载胜利文字
        texture_Map[ResID::Tex_UILossText] = IMG_LoadTexture(renderer, "res/ui_loss_text.png"); // 加载失败文字

		int index = 0;
		for (const auto& pair : texture_Map) // 检查所有纹理是否加载成功
		{
			//printf("Loading texture: %d\n", index++);
            if (!pair.second)
				return false; // 如果加载失败，返回false
		};

		sound_Map[ResID::Sound_ArrowFire_1] = Mix_LoadWAV("res/sound_arrow_fire_1.mp3"); // 加载箭矢发射音效
        sound_Map[ResID::Sound_ArrowFire_2] = Mix_LoadWAV("res/sound_arrow_fire_2.mp3"); // 加载箭矢发射音效
        sound_Map[ResID::Sound_AxeFire] = Mix_LoadWAV("res/sound_axe_fire.wav"); // 加载斧头发射音效
        sound_Map[ResID::Sound_ShellFire] = Mix_LoadWAV("res/sound_shell_fire.wav"); // 加载炮弹发射音效
        sound_Map[ResID::Sound_ArrowHit_1] = Mix_LoadWAV("res/sound_arrow_hit_1.mp3"); // 加载箭矢命中音效
        sound_Map[ResID::Sound_ArrowHit_2] = Mix_LoadWAV("res/sound_arrow_hit_2.mp3"); // 加载箭矢命中音效
        sound_Map[ResID::Sound_ArrowHit_3] = Mix_LoadWAV("res/sound_arrow_hit_3.mp3"); // 加载箭矢命中音效
        sound_Map[ResID::Sound_AxeHit_1] = Mix_LoadWAV("res/sound_axe_hit_1.mp3"); // 加载斧头命中音效
        sound_Map[ResID::Sound_AxeHit_2] = Mix_LoadWAV("res/sound_axe_hit_2.mp3"); // 加载斧头命中音效
        sound_Map[ResID::Sound_AxeHit_3] = Mix_LoadWAV("res/sound_axe_hit_2.mp3"); // 加载斧头命中音效
        sound_Map[ResID::Sound_ShellHit] = Mix_LoadWAV("res/sound_shell_hit.mp3"); // 加载炮弹命中音效
        
		sound_Map[ResID::Sound_Flash] = Mix_LoadWAV("res/sound_flash.wav"); // 加载特效音效
        sound_Map[ResID::Sound_Impact] = Mix_LoadWAV("res/sound_impact.wav"); // 加载特效音效
        sound_Map[ResID::Sound_Coin] = Mix_LoadWAV("res/sound_coin.mp3"); // 加载金币音效
        sound_Map[ResID::Sound_HomeHurt] = Mix_LoadWAV("res/sound_home_hurt.wav"); // 加载房屋受伤音效
        sound_Map[ResID::Sound_PlaceTower] = Mix_LoadWAV("res/sound_place_tower.mp3"); // 加载放置塔音效
        sound_Map[ResID::Sound_TowerLevelUp] = Mix_LoadWAV("res/sound_tower_level_up.mp3"); // 加载升级塔音效
        sound_Map[ResID::Sound_Win] = Mix_LoadWAV("res/sound_win.wav"); // 加载胜利音效
        sound_Map[ResID::Sound_Loss] = Mix_LoadWAV("res/sound_loss.mp3"); // 加载失败音效
		for (const auto& pair : sound_Map) // 检查所有音效是否加载成功
		{
			//printf("Loading sound: %d\n", index++);
			if (!pair.second)return false; // 如果加载失败，返回false
		};

		music_Map[ResID::Music_BGM] = Mix_LoadMUS("res/music_bgm.mp3"); // 加载背景音乐
		for (const auto& pair : music_Map) // 检查所有音乐是否加载成功
		{
            //printf("Loading music: %d\n", index++);
			if (!pair.second)return false; // 如果加载失败，返回false
		}

		font_Map[ResID::Font_Main] = TTF_OpenFont("res/font.ttf", 25); // 加载主字体

		for (const auto& pair : font_Map) // 检查所有字体是否加载成功
		{
            //printf("Loading font: %d\n", index++);
			if (!pair.second)return false; // 如果加载失败，返回false
		}
		return true;
	}
    const FontMap& GetFontMap() const { return font_Map; }
    const SoundMap& GetSoundMap() const { return sound_Map; }
    const MusicMap& GetMusicMap() const { return music_Map; }
    const TextureMap& GetTextureMap() const { return texture_Map; }

protected:
    ResourcesManager() = default;
	~ResourcesManager() = default;

private:
	FontMap font_Map; // 字体资源映射
    SoundMap sound_Map; // 音效资源映射
    MusicMap music_Map; // 音乐资源映射
    TextureMap texture_Map; // 纹理资源映射

};

// todolml: Del Debug
void Test_ResMgr() {
	ResourcesManager* resMgr = ResourcesManager::GetInstance();
	SDL_Init(SDL_INIT_EVERYTHING);
	Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	SDL_Window* window = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	
	TTF_Init();
	TTF_Font* font = TTF_OpenFont("res/font.ttf", 25);
	if (!font) {
		printf("Failed to load font: %s\n", TTF_GetError());
	}

	//SDL_Texture* ttt = IMG_LoadTexture(renderer, "res/tileset.png"); // 加载纹理
	//if (!ttt) {
	//	printf("Failed to load texture: %s\n", IMG_GetError());
	//};
	resMgr->Load_From_File(renderer);

}
