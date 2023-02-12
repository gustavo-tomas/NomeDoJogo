#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "SDL_include.h"
#include <unordered_map>

using namespace std;

class EventManager {
    public:
        static EventManager& GetInstance();
        void Update();
        bool QuitRequested();
    private:
        EventManager();
        ~EventManager();
        bool quitRequested;
};

#endif // EVENTMANAGER_H