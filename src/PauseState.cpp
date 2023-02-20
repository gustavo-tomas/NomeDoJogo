#include "../header/PauseState.h"
#include "../header/CameraFollower.h"
#include "../header/GameData.h"
#include "../header/InputManager.h"
#include "../header/Text.h"
#include "../header/Sprite.h"

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
    // Background
    GameObject* bgGo = new GameObject();
    Sprite* bg = new Sprite(*bgGo, "./assets/image/ui_background.jpg");
    bg->SetScale(0.56, 0.56); // Original resolution: 1920x1080
    CameraFollower* cf = new CameraFollower(*bgGo);

    bgGo->AddComponent(bg);
    bgGo->AddComponent(cf);
    AddObject(bgGo, -1000);

    // Title
    GameObject* textGo = new GameObject();
    textGo->box.SetVec(Vec2(450, 150));

    CameraFollower* textFollower = new CameraFollower(*textGo, textGo->box.GetVec());
    textGo->AddComponent(textFollower);

    const char* fontFile = "./assets/font/Lena.ttf";
    const char* textStr = "pausa";
    int fontSize = 72;
    Text::TextStyle style = Text::BLENDED;
    SDL_Color color = {255, 255, 255, 255};
    
    Text* text = new Text(*textGo, fontFile, fontSize, style, textStr, color);
    textGo->AddComponent(text);
    
    AddObject(textGo, 20020);

    // Options
    vector<string> options = {"CONTINUAR", "VOLTAR PARA O MENU", "SAIR DO JOGO"};

    for (unsigned i = 0; i < options.size(); i++)
    {
        GameObject* optionsGo = new GameObject();
        optionsGo->box.SetVec(Vec2(450, 250 + 35 * i));

        CameraFollower* optionsFollower = new CameraFollower(*optionsGo, optionsGo->box.GetVec());
        optionsGo->AddComponent(optionsFollower);

        fontFile = "./assets/font/Inder-Regular.ttf";
        textStr = options[i].c_str();
        fontSize = 18;
        style = Text::BLENDED;
        color = {255, 255, 255, 255};
        
        Text* optionsText = new Text(*optionsGo, fontFile, fontSize, style, textStr, color);
        optionsGo->AddComponent(optionsText);
        
        AddObject(optionsGo, 20020);
    }

    // Cursor
    GameObject* cursor = new GameObject();
    cursor->box.SetVec(Vec2(435, 256));

    Sprite* cursorSprite = new Sprite(*cursor, "./assets/image/icons/diamond.png");
    cursorSprite->SetScale(0.06, 0.06);
    cursor->AddComponent(cursorSprite);

    this->cursor = AddObject(cursor, 20020);
}

void PauseState::Update(float dt)
{
    if (QuitRequested() || PopRequested())
        return;

    // Sets quit requested
    if (InputManager::GetInstance().QuitRequested())
        quitRequested = true;

    // Returns to previous state
    if (InputManager::GetInstance().KeyPress(ESCAPE_KEY) || GameData::returnToMenu)
        popRequested = true;

    if (!cursor.expired())
    {
        auto cursorPtr = cursor.lock().get();

        // Cursor displacement
        if (InputManager::GetInstance().KeyPress(DOWN_ARROW_KEY))
            cursorPtr->box.y += 35;

        if (InputManager::GetInstance().KeyPress(UP_ARROW_KEY))
            cursorPtr->box.y -= 35;

        cursorPtr->box.y = max(256.f, cursorPtr->box.y);
        cursorPtr->box.y = min(325.f, cursorPtr->box.y);

        // Resumes
        if (InputManager::GetInstance().KeyPress(ENTER_KEY) && cursorPtr->box.y <= 256)
            popRequested = true;

        // Returns to the menu
        if (InputManager::GetInstance().KeyPress(ENTER_KEY) && cursorPtr->box.y == 291)
            GameData::returnToMenu = true;

        // Quits
        if (InputManager::GetInstance().KeyPress(ENTER_KEY) && cursorPtr->box.y >= 325)
            quitRequested = true;
    }


    // Updates GOs
    UpdateArray(dt);
}

void PauseState::Render()
{
    if (GameData::returnToMenu) return;
    RenderArray();
}

void PauseState::Pause()
{

}

void PauseState::Resume()
{

}
