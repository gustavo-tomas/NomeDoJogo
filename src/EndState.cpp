#include "../header/EndState.h"
#include "../header/CreditState.h"
#include "../header/StageState.h"
#include "../header/Game.h"
#include "../header/GameData.h"
#include "../header/Sprite.h"
#include "../header/GameObject.h"
#include "../header/InputManager.h"
#include "../header/CameraFollower.h"
#include "../header/Text.h"

EndState::EndState()
{
    cout << "\nEnd State\n";
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

    // Cursor displacement
    if (InputManager::GetInstance().KeyPress(DOWN_ARROW_KEY))
        cursor.lock().get()->box.y += 35;

    if (InputManager::GetInstance().KeyPress(UP_ARROW_KEY))
        cursor.lock().get()->box.y -= 35;

    cursor.lock().get()->box.y = max(357.f, cursor.lock().get()->box.y);
    cursor.lock().get()->box.y = min(357.f + 35 * 2, cursor.lock().get()->box.y);

    // Try again
    if (InputManager::GetInstance().KeyPress(ENTER_KEY) && cursor.lock().get()->box.y <= 357)
    {
        popRequested = true;
        Game::GetInstance().Push(new StageState());
    }
    
    // Back to the menu
    else if (InputManager::GetInstance().KeyPress(ENTER_KEY) && cursor.lock().get()->box.y <= 357 + 35 * 1)
    {
        popRequested = true;
        GameData::returnToMenu = true;
    }

    // Credits
    else if (InputManager::GetInstance().KeyPress(ENTER_KEY) && cursor.lock().get()->box.y <= 357 + 35 * 2)
    {
        popRequested = true;
        Game::GetInstance().Push(new CreditState());
    }

    UpdateArray(dt);
}

void EndState::LoadAssets()
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

    AddObject(bgGo, -GameData::WIDTH);

    // Chooses music depending on the ending
    string musicFile;

    if (GameData::playerVictory)
        musicFile = GameData::audiosPath + "musics/victory.mp3";

    else
        musicFile = "./assets/audio/papapa.mp3";

    backgroundMusic = Music(musicFile.c_str(), 15);
    backgroundMusic.Play(-1);

     // Title
    GameObject* titleGo = new GameObject();
    
    Sprite* title = new Sprite(*titleGo, "./assets/image/title.png");
    title->SetScale(0.5, 0.5);

    Vec2 offset = Vec2(GameData::WIDTH / 2.0 - title->GetWidth() / 2.0, GameData::HEIGHT / 2.5 - title->GetHeight() / 2.0);
    CameraFollower* titleCf = new CameraFollower(*titleGo, offset);
    
    titleGo->AddComponent(title);
    titleGo->AddComponent(titleCf);
    AddObject(titleGo, 20020);

    // Options
    GameObject* textGo = new GameObject();

    CameraFollower* textFollower = new CameraFollower(*textGo,Vec2(250, 400));
    textGo->AddComponent(textFollower);
    
    vector<string> options = {"TENTAR NOVAMENTE", "VOLTAR AO MENU", "CRÉDITOS"};

    for (unsigned i = 0; i < options.size(); i++)
    {
        GameObject* textGo = new GameObject();
        textGo->box.SetVec(Vec2(450, 350 + 35 * i));

        CameraFollower* textFollower = new CameraFollower(*textGo, textGo->box.GetVec());
        textGo->AddComponent(textFollower);

        const char* fontFile = "./assets/font/Inder-Regular.ttf";
        const char* textStr = options[i].c_str();
        int fontSize = 18;
        Text::TextStyle style = Text::BLENDED;
        SDL_Color color = {255, 255, 255, 255};
        
        Text* text = new Text(*textGo, fontFile, fontSize, style, textStr, color);
        textGo->AddComponent(text);
        
        AddObject(textGo);
    }

    // Cursor
    GameObject* cursor = new GameObject();
    cursor->box.SetVec(Vec2(435, 357));

    Sprite* cursorSprite = new Sprite(*cursor, "./assets/image/icons/diamond.png");
    cursorSprite->SetScale(0.06, 0.06);
    cursor->AddComponent(cursorSprite);

    this->cursor = AddObject(cursor, 20020);
}

void EndState::Render()
{
    if (GameData::returnToMenu) return;
    RenderArray();
}

void EndState::Pause()
{

}

void EndState::Resume()
{

}
