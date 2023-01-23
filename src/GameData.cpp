#include "../header/GameData.h"

const int GameData::WIDTH = 1024;
const int GameData::HEIGHT = 600;
const char* GameData::TITLE = "O Som Da Lua";

bool GameData::playerVictory = false;
Vec2 GameData::playerPos = Vec2(0, 0);
float GameData::targetFPS = 60.0f;
float GameData::currentFPS = 0.0f;
float GameData::delay = 1000.0f / targetFPS;
