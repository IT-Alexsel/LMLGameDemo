#pragma once
// 地图信息加载与预处理缓存
#include <string>
#include <fstream>
#include <sstream>


#include <iostream>
#include <unordered_map>

#include "tile.h"
#include "route.h"


class Map
{
	// todolml Del Here
	friend int Test_Map_H();
public:
	typedef std::unordered_map<int, Route> SpawnerRoutePool;


	Map() = default;
	~Map() = default;

	bool Load(const std::string& path)
	{
		std::ifstream file(path); // 打开文件
		if (!file.good())
		{
            std::cout << "Error: Cannot open file: " << path << std::endl;
			return false;
		}
		TileMap TileMap_Temp; // 临时地图数据
		int idx_x = -1; // 行
		int idx_y = -1; // 列
		std::string str_line; 
		while (std::getline(file, str_line)) // 逐行读取
		{
			str_line = Trim_Str(str_line); // 去除前后空格
			if (str_line.empty()) continue; // 空单元格跳过

			idx_x = -1, idx_y++;
			TileMap_Temp.emplace_back();

			std::string str_cell;
			std::stringstream str_stream(str_line);
			while (std::getline(str_stream, str_cell, ',')) // 逐个读取单元格
			{	
				idx_x++;
				TileMap_Temp[idx_y].emplace_back();
				Tile& tile = TileMap_Temp[idx_y][idx_x];
				Load_Tile_From_Str(tile, str_cell);
			};

		};
		file.close();
		if (TileMap_Temp.empty() || TileMap_Temp[0].empty()) return false;
		tileMap = TileMap_Temp;

		Generate_Map_Cache();
	};

	size_t Get_Width() const { if (tileMap.empty()) return 0; return tileMap[0].size(); }
    size_t Get_Height() const { return tileMap.size(); }

    const TileMap& Get_TileMap() const { return tileMap; }
    const SpawnerRoutePool& Get_SpawnerRoutePool() const { return spawner_Route_Pool; }
    const SDL_Point& Get_Idx_Home() const { return idx_Home; }

	void Place_Tower(const SDL_Point& idx_tile)
	{
		tileMap[idx_tile.y][idx_tile.x].has_tower = true;
	};

private:
	TileMap tileMap;
	SDL_Point idx_Home = { 0,0 };
	SpawnerRoutePool spawner_Route_Pool;

	std::string Trim_Str(const std::string& str) // 去除字符串前后空格
	{
		return str.substr(str.find_first_not_of(" \t\r\n"), str.find_last_not_of(" \t\r\n") + 1);
	};

	void Load_Tile_From_Str(Tile& tile, const std::string& str)
	{
		std::string str_cell = Trim_Str(str);

		std::stringstream str_stream(str_cell);
		std::string str_value;
		std::vector<int> int_vec;
		while (std::getline(str_stream, str_value, '\\')) // 按\分割截取数据
		{	
			int value;
			try 
			{
				value = std::stoi(str_value); // 字符串转整数
			}
			catch (const std::invalid_argument&)
			{ 
				value = -1;
			}
			int_vec.push_back(value); // 存入数组
		};
        if (int_vec.size() != 4) std::cout<< "Error: Load_Tile_From_Str int_vec.size() != 4: " << std::endl;
        tile.terrain = (int_vec.size() < 1 || int_vec[0] < 0) ? 0 : int_vec[0]; // 地形 默认为 0
        tile.decoration = (int_vec.size() < 2 || int_vec[1] < 0) ? -1 : int_vec[1]; // 装饰 默认为 -1
        tile.direction = (Tile::Direction)((int_vec.size() < 3 || int_vec[2] < -1) ? 0 : int_vec[2]); // 方向 默认为 0
        tile.special_flag = (int_vec.size() <= 3) ? -1 : int_vec[3]; // 特殊标志 默认为 -1
	};

	void Generate_Map_Cache() 
	{
		for (int y = 0; y < Get_Height(); y++)
		{
			for (int x = 0; x < Get_Width(); x++)
			{
				const Tile& tile = tileMap[y][x];
				switch (tile.special_flag)
				{
					case -1: // 无特殊
						break;
                    case 0: // Home 家
						idx_Home.x = x;
						idx_Home.y = y;
						break;
					default: // Spawner 怪物出生点
						spawner_Route_Pool[tile.special_flag] = Route(tileMap, { x,y });
                        break;
				};

			};
		};
	}
};

// todolml Del 删除此测试方法
int Test_Map_H()
{
	std::string path = "./MapData/map.csv";
    Map map = Map();
    map.Load(path);
	map.Generate_Map_Cache();
    std::cout << "idx_Home: " << map.idx_Home.x << " " << map.idx_Home.y << std::endl;
	return 0;

}