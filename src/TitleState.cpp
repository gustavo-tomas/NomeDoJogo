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
    // Music
    backgroundMusic = Music("./assets/audio/musics/main_theme.mp3", 20);
    backgroundMusic.Play(1);

    // Background
    GameObject* bgGo = new GameObject();
    Sprite* bg = new Sprite(*bgGo, "./assets/image/ui_background.jpg");
    bg->SetScale(GameData::BASE_HEIGHT * 1.0f / bg->GetUnscaledHeight(), GameData::BASE_WIDTH * 1.0f / bg->GetUnscaledWidth()); // Original resolution: 1920x1080
    bg->isProportionActive = true;
    CameraFollower* cf = new CameraFollower(*bgGo);

    bgGo->AddComponent(bg);
    bgGo->AddComponent(cf);
    AddObject(bgGo, -GameData::HEIGHT);

    // Title
    GameObject* titleGo = new GameObject();
    title = new Sprite(*titleGo, "./assets/image/title.png");
    title->SetScale(0.7, 0.7);
    // title->SetScale(2, 2);

    Vec2 offset = Vec2(GameData::WIDTH / 2.0 - title->GetWidth() / 2.0, GameData::HEIGHT / 2.5 - title->GetHeight() / 2.0);
    titleCf = new CameraFollower(*titleGo, offset);
    

    titleGo->AddComponent(title);
    titleGo->AddComponent(titleCf);
    AddObject(titleGo);

    // Options
    GameObject* textGo = new GameObject();

    CameraFollower* textFollower = new CameraFollower(*textGo,Vec2(250, 400));
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
    {
        quitRequested = true;
        return;
    }

    // Creates new WorldState
    if (InputManager::GetInstance().KeyPress(SPACE_KEY))
        Game::GetInstance().Push(new WorldState());

    if(GameData::fullscreenUpdateCounter > 0){
        Vec2 offset = Vec2(GameData::WIDTH / 2.0 - title->GetWidth() / 2.0, GameData::HEIGHT / 2.5 - title->GetHeight() / 2.0);
        titleCf->setOffset(offset);
    }
    
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
