#ifndef GAMEDATA_H
#define GAMEDATA_H

class GameData {
    public:
        static const int WIDTH;
        static const int HEIGHT;
        static const char* TITLE;
        static bool playerVictory;
        static float targetFPS;
        static float currentFPS;
        static float delay;
};

#endif // GAMEDATA_H