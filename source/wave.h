#pragma once
#include <vector>

#include "enemy_type.h"

// 映射level.json中的信息

struct Wave {
	struct SpawnEvent { // 生成事件信息
		double interval = 0;
		int spawn_point = 1;
		EnemyType enemy_type = EnemyType::Slim;
	};
	double rewards = 100; // 奖励
	double interval = 3; // 生成间隔
	std::vector<SpawnEvent> spawn_event_list; // 生成事件
};