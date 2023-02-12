#include "../header/InputManager.h"

#include <iostream>

InputManager::InputManager()
{
    quitRequested = false;
    updateCounter = 0;
    mouseX = mouseY = 0;

    for (auto i = 0; i < 6; i++)
    {
        mouseState[i] = false;
        mouseUpdate[i] = 0;
    }
}

InputManager& InputManager::GetInstance()
{
    static InputManager instance;
    return instance;
}

void InputManager::Update()
{
    SDL_Event event;

    SDL_GetMouseState(&mouseX, &mouseY);

    quitRequested = false;
    
    updateCounter++;
    
    // Returns 1 if any event is found, 0 otherwise
    while (!eventQueue.empty())
    {
        event = eventQueue.front();
        eventQueue.pop();
        int key = event.key.keysym.sym;
        int button = event.button.button;
        switch (event.type)
        {
            // If event is quit
            case SDL_QUIT:
                quitRequested = true;
                keyUpdate[key] = updateCounter;
                break;
            
            // If event is key press
            case SDL_KEYDOWN:
                if (event.key.repeat == 1) break;
                keyState[key] = true;
                keyUpdate[key] = updateCounter;
                break;

            // If event is key release
            case SDL_KEYUP:
                keyState[key] = false;
                keyUpdate[key] = updateCounter;
                break;

            // If event is mouse button down
            case SDL_MOUSEBUTTONDOWN:
                mouseState[button] = true;
                mouseUpdate[button] = updateCounter;
                break;

            // If event is mouse button release
            case SDL_MOUSEBUTTONUP:
                mouseState[button] = false;
                mouseUpdate[button] = updateCounter;
                break;

            default:
                break;
        }
    }
}

bool InputManager::KeyPress(int key)
{
    return (keyState[key] && keyUpdate[key] == updateCounter);
}

bool InputManager::KeyRelease(int key)
{
    return (!keyState[key] && keyUpdate[key] == updateCounter);
}

bool InputManager::IsKeyDown(int key)
{
    return keyState[key];
}

bool InputManager::MousePress(int button)
{
    return (mouseState[button] && mouseUpdate[button] == updateCounter);
}

bool InputManager::MouseRelease(int button)
{
    return (!mouseState[button] && mouseUpdate[button] == updateCounter);
}

bool InputManager::IsMouseDown(int button)
{
    return mouseState[button];
}

int InputManager::GetMouseX()
{
    return mouseX;
}

int InputManager::GetMouseY()
{
    return mouseY;
}

bool InputManager::QuitRequested()
{
    return quitRequested;
}

bool InputManager::AddEvent(SDL_Event &event){
    eventQueue.push(event);
}

InputManager::~InputManager()
{

}