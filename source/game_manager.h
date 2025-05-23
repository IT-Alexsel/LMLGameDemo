#pragma once
#include "manager.h"
#include "config_manager.h"
#include "resources_manager.h"
#include "enemy_manager.h"
#include "wave_manager.h"
#include "tower_manager.h"
#include "bullet_manager.h"
#include "coin_manager.h"
#include "status_bar.h"
#include "place_panel.h"
#include "upgrade_panel.h"
#include "panel.h"
#include "player_manager.h"
#include "banner.h"
#include "timer_manager.h"

#include <SDL.h>
#include <SDL_ttf.h> // 字体处理
#include <SDL_image.h> // 图片处理
#include <SDL_mixer.h> // 音频处理
#include <SDL2_gfxPrimitives.h> // 绘制图形


const std::string LevelPath =	"./MapData/level.json";
const std::string GamePath =	"./MapData/config.json";
const std::string MapPath =		"./MapData/map.csv";


class GameManager : public SingleMgr<GameManager>
{
	friend class SingleMgr<GameManager>; // 允许SingleMgr访问私有成员
public:
	int Run(int argc, char** argv) // 游戏主循环
	{	
		static TimerManager* timer_mgr = TimerManager::GetInstance();
		Mix_FadeInMusic(ResourcesManager::GetInstance()->GetMusicMap().find(ResID::Music_BGM)->second, -1, 2000); // 淡入背景音乐

		Uint64 last_count = SDL_GetPerformanceCounter(); // 获取当前计数(高性能下计数器的计数)
		Uint64 counter_freq = SDL_GetPerformanceFrequency(); // 获取计数频率(每秒高性能下计数器的计数)
		Uint64 current_counter; // 当前帧计数

		while (isRunning)
		{
			while (SDL_PollEvent(&event))
			{
				On_Input();
			};

			// 限制帧率: (当前帧计数-上一帧计数)/秒计数频率
			current_counter = SDL_GetPerformanceCounter();
			double delta_time = (double)(current_counter - last_count) / counter_freq;// 时间间隔
			last_count = current_counter;
			if (delta_time * 1000 < 1000.0 / fps) {
				SDL_Delay((Uint32)(1000.0 / fps - delta_time * 1000)); // 延迟
			};
			if (timer_mgr->Is_Pause()) continue; // 如果计时器暂停则跳过
			// 更新
			Update(delta_time);

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 设置渲染器颜色
			SDL_RenderClear(renderer); // 清空渲染器
			// 绘制画面
            Render();
            SDL_RenderPresent(renderer); // 显示渲染器
		};
		return 0;
	};

protected:
	GameManager() {
		InitSDL();
		// 初始化配置(地图、关卡、资源)
		ConfigManager* config_mgr = ConfigManager::GetInstance();
		Init_Assert(config_mgr->map.Load(MapPath), u8"ConfigManager:Load_Map Error");
		Init_Assert(config_mgr->Load_Level_Config(LevelPath), u8"ConfigManager:Load_Level_Config Error");
        Init_Assert(config_mgr->Load_Game_Config(GamePath), u8"ConfigManager:Load_Game_Config Error");
		// 初始化窗口
		Init_Window(config_mgr->basic_template.title.c_str(), config_mgr->basic_template.width, config_mgr->basic_template.height);
		// 初始化渲染器
		Init_Renderer();
		// 初始化资源
        ResourcesManager* res_mgr = ResourcesManager::GetInstance();
        Init_Assert(res_mgr->Load_From_File(renderer), u8"ResourcesManager:Load_From_File Error");

		Init_Assert(Generate_Tile_Map_Texture(), u8"Generate_Tile_Map_Texture Error");

		// 初始化UI状态栏
		status_bar.Set_Position(15, 15);
        // 初始化UI面板
        place_panel = new PlacePanel();
		upgrade_panel = new UpgradePanel();
		// 初始化结算
        banner = new Banner();
	};
	~GameManager() {
		SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
		TTF_Quit();
        Mix_Quit();
        IMG_Quit();
        SDL_Quit();
	};
private:
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	bool isRunning = true;
	SDL_Event event;
    int fps = 60;

	SDL_Texture* tex_tile_map = NULL;

	StatusBar status_bar; // 状态栏
	Panel* place_panel = NULL; // 放置面板
    Panel* upgrade_panel = NULL; // 升级面板
	Banner* banner; // 结算

	

	// -------------初始化--------------
	void InitSDL()
	{
		Init_Assert(!SDL_Init(SDL_INIT_EVERYTHING), u8"SDL_Init Error"); // 初始化SDL
        Init_Assert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG), u8"IMG_Init Error"); // 初始化IMG
        Init_Assert(Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3), u8"MIX_Init Error"); // 初始化MIX
		Init_Assert(!TTF_Init(), u8"TTF_Init Error"); // 初始化TTF
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048); // 初始化音频
		// 设置SDL提示，显示输入法 todolml
		SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1"); 
	};
	void Init_Window(const char* title, const int& width, const int& height)
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
		Init_Assert(window, u8"SDL_CreateWindow Error");
	};
	void Init_Renderer()
	{
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);// 创建渲染器(硬件加速、垂直同步、渲染目标为纹理)
        Init_Assert(renderer, u8"SDL_CreateRenderer Error");
	};
	void Init_Assert(bool flag, const char* error)
	{
		if (flag)
		{
			return;
		};
		std::cout << flag << "-----" << error << ":" << SDL_GetError() << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"错误", error, window);
		exit(-1);
	};

	bool Generate_Tile_Map_Texture() // 生成地图纹理 todo lml
	{ 
		const Map& map = ConfigManager::GetInstance()->map;
		const TileMap& tileMap = map.Get_TileMap();
		SDL_Rect& rect_tile_map = ConfigManager::GetInstance()->rect_tile_map;
		SDL_Texture* tex_tile_set = ResourcesManager::GetInstance()->GetTextureMap().find(ResID::Tex_Tileset)->second;

		int width_tex_tile_set, height_tex_tile_set;
		SDL_QueryTexture(tex_tile_set, NULL, NULL, &width_tex_tile_set, &height_tex_tile_set); // 获取纹理大小
		int num_tile_single_line =  (int)std::ceil((double)width_tex_tile_set / SIZE_TILE); // 每行格子数
		
		// 瓦片地图纹理尺寸
		int width_tex_tile_map, height_tex_tile_map;
        width_tex_tile_map = (int)map.Get_Width() * SIZE_TILE;
        height_tex_tile_map = (int)map.Get_Height() * SIZE_TILE;

		tex_tile_map = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 
			width_tex_tile_map, height_tex_tile_map); // 创建瓦片地图纹理

		if (!tex_tile_map)
		{
			return false;
		}
		ConfigManager* config_mgr = ConfigManager::GetInstance();
		rect_tile_map.x = config_mgr->basic_template.width / 2 - width_tex_tile_map / 2;
		rect_tile_map.y = config_mgr->basic_template.height / 2 - height_tex_tile_map / 2;
        rect_tile_map.w = width_tex_tile_map;
        rect_tile_map.h = height_tex_tile_map;


        SDL_SetTextureBlendMode(tex_tile_map, SDL_BLENDMODE_BLEND); // 设置纹理混合模式
        SDL_SetRenderTarget(renderer, tex_tile_map); // 设置渲染目标为瓦片地图纹理

		for (int y = 0; y < map.Get_Height(); y++)
		{
			for (int x = 0; x < map.Get_Width(); x++)
			{
				SDL_Rect rect_src; // 源矩形
				const Tile& tile = tileMap[y][x];

				const SDL_Rect& rect_dst = { x * SIZE_TILE, y * SIZE_TILE, SIZE_TILE, SIZE_TILE }; // 目标矩形
				rect_src = 
				{
					(tile.terrain % num_tile_single_line) * SIZE_TILE,
                    (tile.terrain / num_tile_single_line) * SIZE_TILE,
                    SIZE_TILE,SIZE_TILE
				};
                SDL_RenderCopy(renderer, tex_tile_set, &rect_src, &rect_dst); // 绘制瓦片

                if (tile.decoration >= 0)
				{
					rect_src = 
					{
						(tile.decoration % num_tile_single_line) * SIZE_TILE,
                        (tile.decoration / num_tile_single_line) * SIZE_TILE,
                        SIZE_TILE,SIZE_TILE
					};
                    SDL_RenderCopy(renderer, tex_tile_set, &rect_src, &rect_dst); // 绘制装饰
				
				}
			}
		}
		const SDL_Point& idx_home = map.Get_Idx_Home();
		const SDL_Rect& rect_dst =
		{
			idx_home.x * SIZE_TILE,
            idx_home.y * SIZE_TILE,
            SIZE_TILE,SIZE_TILE
		};
		SDL_RenderCopy(renderer, ResourcesManager::GetInstance()->GetTextureMap().find(ResID::Tex_Home)->second, nullptr, &rect_dst); // 绘制房屋
		SDL_SetRenderTarget(renderer, nullptr); // 恢复渲染目标为默认渲染目标
		return true;
	};

    // -------------逻辑处理--------------
	void On_Input()
	{
		static SDL_Point pos_center;
		static SDL_Point idx_tile_selected;
		static ConfigManager* config_mgr = ConfigManager::GetInstance();
		static TimerManager* timer_mgr = TimerManager::GetInstance();
		static const ResourcesManager::SoundMap& sound_map = ResourcesManager::GetInstance()->GetSoundMap();
		
		switch (event.type)
		{
			case SDL_QUIT: // 退出事件
				isRunning = false;
				break;
            case SDL_MOUSEBUTTONDOWN: // 鼠标点击事件
				if (config_mgr->is_game_over) break;
				if (Get_Cursor_Idx_Tile(event.motion.x, event.motion.y, idx_tile_selected))
				{
					Get_Selected_Tile_Center_Pos(idx_tile_selected, pos_center);

					if (Check_Home(idx_tile_selected))
					{
						upgrade_panel->Set_Idx_Tile(idx_tile_selected);
                        upgrade_panel->Set_Center_Pos(pos_center);
                        upgrade_panel->Show();
					}
					else if (Can_Place_Tower(idx_tile_selected))
					{
						place_panel->Set_Idx_Tile(idx_tile_selected);
                        place_panel->Set_Center_Pos(pos_center);
                        place_panel->Show();
					}
				}
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
                    case SDLK_p:
						timer_mgr->Change_Pause_State();
						break;
					case SDLK_g:
						CoinManager::GetInstance()->Increase_Coin(100);
						Mix_PlayChannel(-1, sound_map.find(ResID::Sound_Coin)->second, 0);
                        break;

				}
                break;

		default:
			break;
		}

		if (!config_mgr->is_game_over)
		{
            PlayerManager::GetInstance()->On_Input(&event);
			place_panel->On_Input(event);
            upgrade_panel->On_Input(event);
			
		}

	};
	void Update(double delta_time)
	{
		static bool is_game_over_last_tick = false;
		
		static ConfigManager* config_mgr = ConfigManager::GetInstance();

		if (!config_mgr->is_game_over)
		{
			status_bar.Update(renderer);
			place_panel->Update(renderer);
            upgrade_panel->Update(renderer);
			WaveManager::GetInstance()->Update(delta_time);
			EnemyManager::GetInstance()->Update(delta_time);
			BulletManager::GetInstance()->Update(delta_time);
            TowerManager::GetInstance()->Update(delta_time);
			CoinManager::GetInstance()->Update(delta_time);
			PlayerManager::GetInstance()->Update(delta_time);
			return;
		}

		if (!is_game_over_last_tick && config_mgr->is_game_over) // 是否为游戏结束帧
		{
			static const ResourcesManager::SoundMap& sound_map = ResourcesManager::GetInstance()->GetSoundMap();
			Mix_FadeOutMusic(1500);
            Mix_PlayChannel(-1, sound_map.find(config_mgr->is_game_win ? ResID::Sound_Win : ResID::Sound_Loss)->second, 0);
		}

		is_game_over_last_tick = config_mgr->is_game_over;

		banner->Update(delta_time);
		if (banner->Check_End_Display())
		{
			isRunning = false;
		}
	};
	void Render()
	{
        static ConfigManager* config_mgr = ConfigManager::GetInstance();
		static SDL_Rect& rect_dst = config_mgr->rect_tile_map;
        SDL_RenderCopy(renderer, tex_tile_map, nullptr, &rect_dst); // 绘制瓦片地图
		
		EnemyManager::GetInstance()->Render(renderer); // 绘制敌人
        BulletManager::GetInstance()->Render(renderer); // 绘制子弹
        TowerManager::GetInstance()->Render(renderer); // 绘制塔
		CoinManager::GetInstance()->Render(renderer); // 绘制金币
		PlayerManager::GetInstance()->Render(renderer); // 绘制玩家

		if (!config_mgr->is_game_over)
		{
			status_bar.Render(renderer);
            place_panel->Render(renderer);
            upgrade_panel->Render(renderer);
			return;
		}

		int width_screen, hight_screen;
		SDL_GetWindowSizeInPixels(window, &width_screen, &hight_screen);
		banner->Set_Center_Position({ (double)width_screen / 2, (double)hight_screen / 2 });
        banner->Render(renderer);
	};

	bool Check_Home(const SDL_Point& idx_tile)
	{
		static const Map& map = ConfigManager::GetInstance()->map;
		static const SDL_Point& idx_home = map.Get_Idx_Home();
        
		return (idx_tile.x == idx_home.x && idx_tile.y == idx_home.y);
	}

	bool Get_Cursor_Idx_Tile(int screen_x, int screen_y, SDL_Point& out_idx_tile_select) // 获取鼠标位置对应的瓦片索引
	{
		static const Map& map = ConfigManager::GetInstance()->map;
		static const SDL_Rect& rect_tile_map = ConfigManager::GetInstance()->rect_tile_map;

		// 判断鼠标位置是否在瓦片地图内
		if (screen_x < rect_tile_map.x ||
			screen_x > rect_tile_map.x + rect_tile_map.w ||
			screen_y < rect_tile_map.y ||
			screen_y > rect_tile_map.y + rect_tile_map.h) return false;

		out_idx_tile_select.x = std::min((screen_x - rect_tile_map.x) / SIZE_TILE, (int)map.Get_Width() - 1);
        out_idx_tile_select.y = std::min((screen_y - rect_tile_map.y) / SIZE_TILE, (int)map.Get_Height() - 1);
		return true;
	}

	bool Can_Place_Tower(const SDL_Point& idx_tile) const // 判断是否可以放置塔
	{
        static const Map& map = ConfigManager::GetInstance()->map;
		const Tile& tile = map.Get_TileMap()[idx_tile.y][idx_tile.x];

		return (tile.decoration < 0 && tile.direction == Tile::Direction::None && !tile.has_tower);
	}

	void Get_Selected_Tile_Center_Pos(const SDL_Point& idx_tile_selected, SDL_Point& out_pos) const // 获取选中瓦片的中心位置(放置塔)
	{
		static const SDL_Rect& rect_tile_map = ConfigManager::GetInstance()->rect_tile_map;
		out_pos.x = rect_tile_map.x + idx_tile_selected.x * SIZE_TILE + SIZE_TILE / 2;
		out_pos.y = rect_tile_map.y + idx_tile_selected.y * SIZE_TILE + SIZE_TILE / 2;

	}
};

