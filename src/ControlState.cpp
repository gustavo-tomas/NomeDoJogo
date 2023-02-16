#include "../header/ControlState.h"
#include "../header/GameObject.h"
#include "../header/Sprite.h"
#include "../header/InputManager.h"
#include "../header/CameraFollower.h"
#include "../header/Text.h"
#include "../header/GameData.h"

ControlState::ControlState() : State()
{
    cout << "\nControls State\n";
}

ControlState::~ControlState()
{
    objectArray.clear();
    cout << "ControlState deleted successfully!\n";
}

void ControlState::Start()
{
    LoadAssets();
    StartArray();
    started = true;
}

void ControlState::LoadAssets()
{
    // Background
    GameObject* bgGo = new GameObject();
    Sprite* bg = new Sprite(*bgGo, "./assets/image/ui_background.jpg");
    bg->SetScale(GameData::BASE_HEIGHT * 1.0f / bg->GetUnscaledHeight(), GameData::BASE_WIDTH * 1.0f / bg->GetUnscaledWidth()); // Original resolution: 1920x1080
    bg->isProportionActive = true;
    CameraFollower* cf = new CameraFollower(*bgGo);

    bgGo->AddComponent(bg);
    bgGo->AddComponent(cf);
    AddObject(bgGo, -GameData::HEIGHT);

    // Controls
    string titleFont = "Lena.ttf";
    string nameFont = "Inder-Regular.ttf";

    int titleSize = 64, nameSize = 22;

    AddText("Controles\n", GameData::fontsPath + titleFont, titleSize, Vec2(200, 150));

    // Programmers
    AddText("Mover: W, A, S, D\n", GameData::fontsPath + titleFont, nameSize, Vec2(250, 250));
    AddText("Atirar: SPACE\n", GameData::fontsPath + titleFont, nameSize, Vec2(250, 280));
    AddText("Interagir: E\n", GameData::fontsPath + titleFont, nameSize, Vec2(250,310));
    AddText("Pausa (Modo Batalha): ESC\n", GameData::fontsPath + titleFont, nameSize, Vec2(250,340));
}

void ControlState::AddText(string text, string fontFile, int fontSize, Vec2 position)
{
    GameObject* textGo = new GameObject();
    textGo->box.SetVec(position);

    CameraFollower* textFollower = new CameraFollower(*textGo, textGo->box.GetVec());
    textGo->AddComponent(textFollower);

    Text::TextStyle style = Text::BLENDED;
    SDL_Color color = {255, 255, 255, 255};
    
    Text* titleText = new Text(*textGo, fontFile.c_str(), fontSize, style, text.c_str(), color);
    textGo->AddComponent(titleText);
    
    AddObject(textGo);
}

void ControlState::Update(float dt)
{
    // Quits
    if (InputManager::GetInstance().QuitRequested())
    {
        quitRequested = true;
        return;
    }
    
    // Returns to title
    if (InputManager::GetInstance().KeyPress(ESCAPE_KEY))
    {
        popRequested = true;
        return;
    }

    UpdateArray(dt);
}

void ControlState::Render()
{
    RenderArray();
}

void ControlState::Pause()
{
}

void ControlState::Resume()
{
}
