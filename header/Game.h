#ifndef GAME_H
#define GAME_H

#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#define INCLUDE_SDL_TTF

#include "SDL_include.h"
#include "State.h"
#include <string>
#include <iostream>
#include <stack>

using namespace std;

class Game {
    public:
        Game(const char* title, int width, int height);
        ~Game();
        static Game& GetInstance();
        SDL_Renderer* GetRenderer();
        State& GetCurrentState();
        void Push(State* state);
        void Run();
        float GetDeltaTime();

    private:
        void CalculateDeltaTime();
        void UpdateDelay(Uint64 start, Uint64 end);
        void UpdateFPS(Uint64 start, Uint64 end);
        int frameStart;
        float dt;
        static Game* instance;
        State* storedState;
        SDL_Window* window;
        SDL_Renderer* renderer;
        stack<unique_ptr<State>> stateStack;
};

#endif // GAME_H