#include "../header/TitleState.h"
#include "../header/GameObject.h"
#include "../header/Sprite.h"
#include "../header/InputManager.h"
#include "../header/Game.h"
#include "../header/StageState.h"
#include "../header/CameraFollower.h"
#include "../header/Text.h"

TitleState::TitleState() : State()
{
    GameObject* titleGo = new GameObject();
    Sprite* titleSprite = new Sprite(*titleGo, "./assets/image/title.png");
    CameraFollower* titleFollower = new CameraFollower(*titleGo);
    titleGo->AddComponent(titleSprite);
    titleGo->AddComponent(titleFollower);

    GameObject* textGo = new GameObject();
    textGo->box.SetVec(Vec2(250, 350));
    CameraFollower* textFollower = new CameraFollower(*textGo, textGo->box.GetVec());
    textGo->AddComponent(textFollower);

    const char* fontFile = "./assets/font/call_me_maybe.ttf";
    const char* textStr = "aperte a tecla space para continuar";
    int fontSize = 32;
    Text::TextStyle style = Text::BLENDED;
    SDL_Color color = {35, 35, 35, 255};
    
    Text* text = new Text(*textGo, fontFile, fontSize, style, textStr, color, 0.75);
    textGo->AddComponent(text);
    
    AddObject(titleGo);
    AddObject(textGo);
}

TitleState::~TitleState()
{
    objectArray.clear();
}

void TitleState::LoadAssets()
{

}

void TitleState::Update(float dt)
{
    // Sets quit requested
    if (InputManager::GetInstance().KeyPress(ESCAPE_KEY) ||
        InputManager::GetInstance().QuitRequested())
        quitRequested = true;

    // Creates new StageState
    if (InputManager::GetInstance().KeyPress(SPACE_KEY))
        Game::GetInstance().Push(new StageState());

    UpdateArray(dt);
}

void TitleState::Render()
{
    RenderArray();
}

void TitleState::Start()
{
    StartArray();
}

void TitleState::Pause()
{

}

void TitleState::Resume()
{

}
