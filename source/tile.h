#pragma once
// 单坐标包含信息
#include <vector>
#define SIZE_TILE 48 // 格子大小

struct Tile
{	
	enum class Direction { // 方向
		None = 0,
		Up, Down, Left, Right
	};
	int terrain = 0; // 地形
	int decoration = 0; // 装饰
	int special_flag = -1; // 特殊标志: -1无;0房屋; 1,2,3,4...怪物出生点
	Direction direction = Direction::None; // 方向（默认不是行进路线）

	// 运行时信息
	bool has_tower = false; // 是否有塔
};

typedef std::vector<std::vector<Tile>> TileMap; // 地图数据结构:二维数组