#ifndef GAMEDATA_H
#define GAMEDATA_H

#include "Vec2.h"
#include <string>

using namespace std;

class GameData {
    public:
        static const int WIDTH;
        static const int HEIGHT;
        static const char* TITLE;
        static bool playerVictory;
        static float targetFPS;
        static float currentFPS;
        static float delay;
        static Vec2 playerPos;
        static const string objectsPath;
        static const string imagesPath;
        static const string fontsPath;
        static const string audiosPath;
        static bool returnToMenu;
};

#endif // GAMEDATA_H
