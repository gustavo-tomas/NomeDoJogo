#include "../header/TitleState.h"
#include "../header/GameObject.h"
#include "../header/Sprite.h"
#include "../header/InputManager.h"
#include "../header/Game.h"
#include "../header/WorldState.h"
#include "../header/CameraFollower.h"
#include "../header/Text.h"
#include "../header/GameData.h"

TitleState::TitleState() : State()
{
    cout << "\nTitle State\n";
}

TitleState::~TitleState()
{
    objectArray.clear();
    cout << "TitleState deleted successfully!\n";
}

void TitleState::Start()
{
    LoadAssets();
    StartArray();
    started = true;
}

void TitleState::LoadAssets()
{
    // Background
    GameObject* bgGo = new GameObject();
    Sprite* bg = new Sprite(*bgGo, "./assets/image/ui_background.jpg");
    bg->SetScale(0.5556, 0.5556); // Original resolution: 1920x1080
    CameraFollower* cf = new CameraFollower(*bgGo);

    bgGo->AddComponent(bg);
    bgGo->AddComponent(cf);
    AddObject(bgGo, -GameData::HEIGHT);

    // Title
    GameObject* titleGo = new GameObject();
    Sprite* title = new Sprite(*titleGo, "./assets/image/title.png");
    title->SetScale(0.7, 0.7);

    Vec2 offset = Vec2(GameData::WIDTH / 2.0 - title->GetWidth() / 2.0, GameData::HEIGHT / 2.5 - title->GetHeight() / 2.0);
    CameraFollower* titleCf = new CameraFollower(*titleGo, offset);

    titleGo->AddComponent(title);
    titleGo->AddComponent(titleCf);
    AddObject(titleGo);

    // Options
    GameObject* textGo = new GameObject();
    textGo->box.SetVec(Vec2(250, 400));

    CameraFollower* textFollower = new CameraFollower(*textGo, textGo->box.GetVec());
    textGo->AddComponent(textFollower);

    const char* fontFile = "./assets/font/Inder-Regular.ttf";
    const char* textStr = "Aperte a tecla SPACE para continuar";
    int fontSize = 32;
    Text::TextStyle style = Text::BLENDED;
    SDL_Color color = {255, 255, 255, 255};
    
    Text* text = new Text(*textGo, fontFile, fontSize, style, textStr, color);
    textGo->AddComponent(text);
    
    AddObject(textGo);
}

void TitleState::Update(float dt)
{
    // Sets quit requested
    if (InputManager::GetInstance().KeyPress(ESCAPE_KEY) ||
        InputManager::GetInstance().QuitRequested())
    {
        quitRequested = true;
        return;
    }

    // Creates new WorldState
    if (InputManager::GetInstance().KeyPress(SPACE_KEY))
        Game::GetInstance().Push(new WorldState());

    UpdateArray(dt);
}

void TitleState::Render()
{
    RenderArray();
}

void TitleState::Pause()
{

}

void TitleState::Resume()
{

}
