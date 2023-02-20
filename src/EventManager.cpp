#include "../header/EventManager.h"
#include "../header/InputManager.h"
#include "../header/Game.h"
#include "../header/GameData.h"

#include <iostream>

EventManager::EventManager()
{

}

EventManager& EventManager::GetInstance()
{
    static EventManager instance;
    return instance;
}

void EventManager::Update()
{
    SDL_Event event;

    auto& inputManager = InputManager::GetInstance(); 
    
    // Returns 1 if any event is found, 0 otherwise
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            // If event is quit
            case SDL_QUIT:
                quitRequested = true;
                break;

            // If event is a window evennt
            case SDL_WINDOWEVENT:
                if (event.type == SDL_WINDOW_RESIZABLE)
                {
                    SDL_Window* win = SDL_GetWindowFromID(event.window.windowID);
                    if (win == (SDL_Window*)Game::GetInstance().GetWindow())
                    {
                        SDL_GetWindowSize(win, &GameData::WIDTH, &GameData::HEIGHT);
                    }
                }
                break;
            
            // If event is a inputType
            case SDL_KEYDOWN:
            case SDL_KEYUP:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                inputManager.AddEvent(event);
                break;

            default:
                break;
        }
    }
}


EventManager::~EventManager()
{

}