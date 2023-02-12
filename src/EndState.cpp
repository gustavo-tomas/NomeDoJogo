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
        bgFile = "./assets/image/ui_background.jpg";

    else
        bgFile = "./assets/image/ui_background.jpg";
    
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
    started = true;
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
    string musicFile;

    if (GameData::playerVictory)
        musicFile = GameData::audiosPath + "musics/victory.mp3";

    else
        musicFile = "./assets/audio/papapa.mp3";

    backgroundMusic = Music(musicFile.c_str(), 15);
    backgroundMusic.Play(-1);

    // Writes text
    GameObject* textGo = new GameObject();
    textGo->box.SetVec(Vec2(25, 350));
    CameraFollower* textFollower = new CameraFollower(*textGo, textGo->box.GetVec());
    textGo->AddComponent(textFollower);

    const char* fontFile = "./assets/font/Inder-Regular.ttf";
    const char* textStr = "Aperte a tecla ESC para sair do jogo ou SPACE para voltar ao inicio";
    int fontSize = 32;
    Text::TextStyle style = Text::BLENDED;
    SDL_Color color = {255, 255, 255, 255};
    unsigned int wrappingLength = 800;
    
    Text* text = new Text(*textGo, fontFile, fontSize, style, textStr, color, wrappingLength);
    textGo->AddComponent(text);

    AddObject(textGo, 10020);
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
