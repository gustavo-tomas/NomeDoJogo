#include "../header/CreditState.h"
#include "../header/GameObject.h"
#include "../header/Sprite.h"
#include "../header/InputManager.h"
#include "../header/CameraFollower.h"
#include "../header/Text.h"
#include "../header/GameData.h"

CreditState::CreditState() : State()
{
    cout << "\nCredits State\n";
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
    // Music
    backgroundMusic = Music((GameData::audiosPath + "musics/tree.mp3").c_str());
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

    // Credits
    string programmers = "Davi\n" "Gustavo\n" "Oscar\n";
    string designers = "Eduardo\n" "Rafael\n" "Rafael\n";
    string musicians = "Letícia\n" "Luan\n";
    
    string titleFont = "Lena.ttf";
    string nameFont = "Inder-Regular.ttf";

    int titleSize = 64, subtitleSize = 36, nameSize = 22;

    AddText("Som Da Luna\n", GameData::fontsPath + titleFont, titleSize, Vec2(350, 350));

    // Programmers
    AddText("Programacao\n", GameData::fontsPath + titleFont, subtitleSize, Vec2(450, 650));
    AddText(programmers, GameData::fontsPath + nameFont, nameSize, Vec2(450, 695));

    // Designers
    AddText("Design\n", GameData::fontsPath + titleFont, subtitleSize, Vec2(450, 835));
    AddText(designers, GameData::fontsPath + nameFont, nameSize, Vec2(450, 880));

    // Musicians
    AddText("Musica/SFX\n", GameData::fontsPath + titleFont, subtitleSize, Vec2(450, 1020));
    AddText(musicians, GameData::fontsPath + nameFont, nameSize, Vec2(450, 1065));

    // Thank you
    AddText("Obrigado por jogar o nosso jogo!\n", GameData::fontsPath + titleFont, subtitleSize, Vec2(250, 1450));
}

void CreditState::AddText(string text, string fontFile, int fontSize, Vec2 position)
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

void CreditState::Update(float dt)
{
    // Sets quit requested
    if (InputManager::GetInstance().KeyPress(ESCAPE_KEY))
    {
        popRequested = true;
        return;
    }

    scrollTimer.Update(dt);

    if (scrollTimer.Get() >= 38.0)
        backgroundMusic.Stop(1500);

    // Scrolling
    if (scrollTimer.Get() < 37.0)
    {
        for (auto& object : objectArray)
        {
            Text* text = (Text*) object->GetComponent("Text");
            if (text == nullptr) continue;

            CameraFollower* follower = (CameraFollower *) object->GetComponent("CameraFollower");
            if (follower == nullptr) continue;
            
            follower->setOffset(object->box.GetVec() + Vec2{0, -0.5});
        }
    }

    UpdateArray(dt);
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
