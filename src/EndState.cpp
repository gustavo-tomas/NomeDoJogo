#include "../header/EndState.h"
#include "../header/GameData.h"
#include "../header/Sprite.h"
#include "../header/GameObject.h"
#include "../header/InputManager.h"
#include "../header/CameraFollower.h"
#include "../header/Text.h"

EndState::EndState()
{
    // Chooses background depending on the ending
    const char* bgFile;

    if (GameData::playerVictory)
        bgFile = "./assets/image/background.png";

    else
        bgFile = "./assets/image/background.png";
    
    GameObject* bgGo = new GameObject();
    Sprite* bg = new Sprite(*bgGo, bgFile);
    CameraFollower* endFollower = new CameraFollower(*bgGo);
    bgGo->AddComponent(bg);
    bgGo->AddComponent(endFollower);

    AddObject(bgGo);
}

EndState::~EndState()
{
    objectArray.clear();
}

void EndState::Start()
{
    LoadAssets();
    StartArray();
}

void EndState::Update(float dt)
{
    if (InputManager::GetInstance().QuitRequested() ||
        InputManager::GetInstance().KeyPress(ESCAPE_KEY))
        quitRequested = true;

    if (InputManager::GetInstance().KeyPress(SPACE_KEY))
        popRequested = true;
    
    UpdateArray(dt);
}

void EndState::LoadAssets()
{
    // Chooses music depending on the ending
    const char* musicFile;

    if (GameData::playerVictory)
        musicFile = "./assets/audio/papapa.mp3";

    else
        musicFile = "./assets/audio/doom.mp3";

    backgroundMusic = Music(musicFile);
    backgroundMusic.Play(-1);

    // Writes text
    GameObject* textGo = new GameObject();
    textGo->box.SetVec(Vec2(25, 350));
    CameraFollower* textFollower = new CameraFollower(*textGo, textGo->box.GetVec());
    textGo->AddComponent(textFollower);

    const char* fontFile = "./assets/font/call_me_maybe.ttf";
    const char* textStr = "aperte a tecla esc para sair do jogo ou space para voltar ao inicio";
    int fontSize = 32;
    Text::TextStyle style = Text::BLENDED;
    SDL_Color color = {195, 35, 35, 255};
    
    Text* text = new Text(*textGo, fontFile, fontSize, style, textStr, color);
    textGo->AddComponent(text);

    AddObject(textGo);
}

void EndState::Render()
{
    RenderArray();
}

void EndState::Pause()
{

}

void EndState::Resume()
{

}
