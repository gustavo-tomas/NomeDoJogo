#include "../header/DialogBox.h"
#include "../header/CameraFollower.h"
#include "../header/Sprite.h"
#include "../header/Text.h"
#include "../header/Game.h"

DialogBox::DialogBox(GameObject& associated, string dialogText) : Component(associated)
{
    // Sprite
    Sprite* sprite = new Sprite(associated, "./assets/image/box.png");

    // CameraFollower
    CameraFollower* cf = new CameraFollower(associated, associated.box.GetVec());

    // Text
    const char* fontFile = "./assets/font/call_me_maybe.ttf";
    int fontSize = 16;
    Text::TextStyle style = Text::BLENDED;
    SDL_Color color = {212, 15, 15, 255};
    
    Text* text = new Text(associated, fontFile, fontSize, style, dialogText.c_str(), color);
    
    associated.AddComponent(sprite);
    associated.AddComponent(cf);
    associated.AddComponent(text);
}

void DialogBox::Update(float dt)
{
    cout << "POS " << associated.box.GetVec().x << " " << associated.box.GetVec().y << endl;
}

void DialogBox::Render()
{
    
}

bool DialogBox::Is(const char* type)
{
    string str_type = type; 
    return str_type == "DialogBox";
}
