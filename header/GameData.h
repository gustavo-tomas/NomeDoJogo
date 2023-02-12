#ifndef GAMEDATA_H
#define GAMEDATA_H

#include "Vec2.h"
#include <string>

using namespace std;

class GameData {
    public:
        static int BASE_WIDTH;
        static int BASE_HEIGHT;
        static int PREV_WIDTH;
        static int PREV_HEIGHT;
        static int WIDTH;
        static int HEIGHT;
        static const char* TITLE;
        static bool playerVictory;
        static bool isFullScreen;
        static float targetFPS;
        static float currentFPS;
        static float delay;
        static Vec2 playerPos;
        static const string objectsPath;

        static int32_t fullscreenUpdateCounter;
};

#endif // GAMEDATA_H