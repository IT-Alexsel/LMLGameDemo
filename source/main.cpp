#define SDL_MAIN_HANDLED
#include <iostream>
#include <cstdlib>

#include "manager.h"
#include "game_manager.h"
#include "map.h"
#include "config_manager.h"
#include "resources_manager.h"


int main(int argc, char** argv) {
	
	return GameManager::GetInstance()->Run(argc, argv);
};