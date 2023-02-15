#include "../header/CreditState.h"
#include "../header/GameObject.h"
#include "../header/Sprite.h"
#include "../header/InputManager.h"
#include "../header/Game.h"
#include "../header/WorldState.h"
#include "../header/Camera.h"
#include "../header/CameraFollower.h"
#include "../header/Text.h"
#include "../header/GameData.h"
#include "../header/Sound.h"

CreditState::CreditState() : State()
{
    cout << "\nTitle State\n";
}

CreditState::~CreditState()
{
    objectArray.clear();
    cout << "CreditState deleted successfully!\n";
}

void CreditState::Start()
{
    LoadAssets();
    StartArray();
    started = true;
}

void CreditState::LoadAssets()
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

    // Title
    // GameObject* titleGo = new GameObject();
    
    // auto title = new Sprite(*titleGo, "./assets/image/title.png");
    // title->SetScale(0.5, 0.5);

    // Vec2 offset = Vec2(GameData::WIDTH / 2.0 - title->GetWidth() / 2.0, GameData::HEIGHT / 2.5 - title->GetHeight() / 2.0);
    // auto titleCf = new CameraFollower(*titleGo, offset);
    

    // titleGo->AddComponent(title);
    // titleGo->AddComponent(titleCf);
    // AddObject(titleGo);

    // Options
    // GameObject* textGo = new GameObject();

    // CameraFollower* textFollower = new CameraFollower(*textGo,Vec2(250, 400));
    // textGo->AddComponent(textFollower);
    
    


    GameObject* textGo = new GameObject();
    textGo->box.SetVec(Vec2(450, 350 ));

    CameraFollower* textFollower = new CameraFollower(*textGo, textGo->box.GetVec());
    textGo->AddComponent(textFollower);

    const char* fontFile = "./assets/font/Inder-Regular.ttf";
    const char* textStr = "Créditos\n"
                            "Programadores\n"
                            "Davi\n"
                            "Gustavo\n"
                            "Oscar\n"
                            "Designer\n"
                            "Eduardo\n"
                            "Rafael\n"
                            "Rafael\n"
                            "Musicos\n"
                            "Letícia\n"
                            "Luan\n"
                            ;
    int fontSize = 18;
    Text::TextStyle style = Text::BLENDED;
    SDL_Color color = {255, 255, 255, 255};
    
    Text* text = new Text(*textGo, fontFile, fontSize, style, textStr, color);
    textGo->AddComponent(text);
    
    AddObject(textGo);

}

void CreditState::Update(float dt)
{
    // Sets quit requested
    if (InputManager::GetInstance().KeyPress(ESCAPE_KEY))
    {
        popRequested = true;
        return;
    }

}

void CreditState::Render()
{
    RenderArray();
}

void CreditState::Pause()
{
}

void CreditState::Resume()
{
}
