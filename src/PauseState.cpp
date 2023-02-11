#include "../header/PauseState.h"
#include "../header/CameraFollower.h"
#include "../header/InputManager.h"
#include "../header/Text.h"

PauseState::PauseState()
{
    cout << "\nPaused\n";
}

PauseState::~PauseState()
{
    objectArray.clear();
    cout << "PauseState deleted successfully!" << endl;
}

void PauseState::Start()
{
    LoadAssets();
    StartArray();
    started = true;
}

void PauseState::LoadAssets()
{
    // Options
    GameObject* textGo = new GameObject();
    textGo->box.SetVec(Vec2(450, 175));

    CameraFollower* textFollower = new CameraFollower(*textGo, textGo->box.GetVec());
    textGo->AddComponent(textFollower);

    const char* fontFile = "./assets/font/Lena.ttf";
    const char* textStr = "pausa";
    int fontSize = 52;
    Text::TextStyle style = Text::BLENDED;
    SDL_Color color = {255, 255, 255, 255};
    
    Text* text = new Text(*textGo, fontFile, fontSize, style, textStr, color);
    textGo->AddComponent(text);
    
    AddObject(textGo, 20020);
}

void PauseState::Update(float dt)
{
    if (QuitRequested() || PopRequested())
        return;

    // Sets quit requested
    if (InputManager::GetInstance().QuitRequested())
        quitRequested = true;

    // Returns to previous state
    if (InputManager::GetInstance().KeyPress(ESCAPE_KEY))
        popRequested = true;

    // Updates GOs
    UpdateArray(dt);
}

void PauseState::Render()
{
    RenderArray();
}

void PauseState::Pause()
{

}

void PauseState::Resume()
{

}
