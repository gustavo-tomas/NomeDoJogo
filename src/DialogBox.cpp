#include "../header/CameraFollower.h"
#include "../header/Game.h"
#include "../header/GameData.h"
#include "../header/DialogBox.h"
#include "../header/Text.h"

DialogBox::DialogBox(GameObject& associated, string title, string text) : Component(associated)
{
    const char* fontFile = "./assets/font/Inder-Regular.ttf";
    Text::TextStyle style = Text::BLENDED;
    SDL_Color color = {255, 255, 255, 255};
    unsigned int wrappingLength = 200;

    // Title
    GameObject* titleGo = new GameObject();
    CameraFollower* titleCf = new CameraFollower(*titleGo, Vec2(GameData::WIDTH - 250, 25));
    Text* titleText = new Text(*titleGo, fontFile, 18, style, title.c_str(), color);
    
    titleGo->AddComponent(titleText);
    titleGo->AddComponent(titleCf);
    Game::GetInstance().GetCurrentState().AddObject(titleGo, 20002);

    // Dialog
    GameObject* dialogGo = new GameObject();
    CameraFollower* dialogCf = new CameraFollower(*dialogGo, Vec2(GameData::WIDTH - 250, 55));
    Text* dialogText = new Text(*dialogGo, fontFile, 14, style, text.c_str(), color, wrappingLength);
    
    dialogGo->AddComponent(dialogText);
    dialogGo->AddComponent(dialogCf);
    Game::GetInstance().GetCurrentState().AddObject(dialogGo, 20002);
}

void DialogBox::Update(float dt)
{

}

void DialogBox::Render()
{
    
}

bool DialogBox::Is(const char* type)
{
    string str_type = type; 
    return str_type == "DialogBox";
}
