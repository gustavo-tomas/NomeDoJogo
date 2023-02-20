#include "../header/CameraFollower.h"
#include "../header/Game.h"
#include "../header/GameData.h"
#include "../header/DialogBox.h"
#include "../header/Text.h"

DialogBox::DialogBox(GameObject& associated, string title, string text, Vec2 pos, int wrappingLength) : Component(associated)
{
    const char* fontFile = "./assets/font/Inder-Regular.ttf";
    Text::TextStyle style = Text::BLENDED;
    SDL_Color color = {255, 255, 255, 255};

    this->title = title;
    this->text = text;
    
    // Title
    GameObject* titleGoInitializer = new GameObject();
    CameraFollower* titleCf = new CameraFollower(*titleGoInitializer, pos);
    Text* titleText = new Text(*titleGoInitializer, fontFile, 18, style, title.c_str(), color);
    
    titleGoInitializer->AddComponent(titleText);
    titleGoInitializer->AddComponent(titleCf);
    titleGo = Game::GetInstance().GetCurrentState().AddObject(titleGoInitializer, 20002);

    // Dialog
    GameObject* dialogGoInitializer = new GameObject();
    CameraFollower* dialogCf = new CameraFollower(*dialogGoInitializer, pos + Vec2(40, 30));
    Text* dialogText = new Text(*dialogGoInitializer, fontFile, 14, style, text.c_str(), color, wrappingLength);
    
    dialogGoInitializer->AddComponent(dialogText);
    dialogGoInitializer->AddComponent(dialogCf);
    dialogGo = Game::GetInstance().GetCurrentState().AddObject(dialogGoInitializer, 20002);
}

DialogBox::~DialogBox()
{
    if (!dialogGo.expired()) dialogGo.lock()->RequestDelete();
    if (!titleGo.expired()) titleGo.lock()->RequestDelete();
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

void DialogBox::Close()
{
    associated.RequestDelete();
}

void DialogBox::SetText(string text)
{
    this->text = text;
    if(!dialogGo.expired())
        ((Text *) dialogGo.lock()->GetComponent("Text"))->SetText(text.c_str());
}

void DialogBox::SetTitle(string title)
{
    this->title = title;
    if(!titleGo.expired())
        ((Text *) titleGo.lock()->GetComponent("Text"))->SetText(title.c_str());
}
