#include "../header/GameData.h"

const int GameData::WIDTH = 1024;
const int GameData::HEIGHT = 600;
const char* GameData::TITLE = "Gustavo Tomas de Paula - 190014148";

bool GameData::playerVictory = false;
float GameData::targetFPS = 60.0f;
float GameData::currentFPS = 0.0f;
float GameData::delay = 1000.0f / targetFPS;
