#include "../header/NPC.h"
#include "../header/Collider.h"
#include "../header/Game.h"
#include "../header/InputManager.h"
#include "../header/GameData.h"

NPC::NPC(GameObject& associated, const char* name, Vec2 pos, Sprite sprite) : Component(associated)
{
    associated.box.SetVec(pos);

    this->name = name;

    Sprite* npcSprite = new Sprite(sprite);
    associated.AddComponent(npcSprite);

    Collider* collider = new Collider(associated);
    associated.AddComponent(collider);
}

void NPC::Update(float dt)
{
    if (talking == true)
        Interact();
    else if
    (
        InputManager::GetInstance().KeyPress(E_KEY) && speechs.size() > currentSpeech &&
        associated.box.GetCenter().GetDistance(GameData::playerPos) < (associated.box.w/2 + associated.box.h/2 + 25) 
    )
    {
        GameObject* textGo = new GameObject();
        dialog = new DialogBox(*textGo, name, speechs[currentSpeech], Vec2(25, GameData::HEIGHT - 100), 1000);
        textGo->AddComponent(dialog);
        Game::GetInstance().GetCurrentState().AddObject(textGo);
        talking = true;
    }

}

void NPC::Render(){}

bool NPC::Is(const char* type)
{
    string str_type = type;
    return (str_type == "NPC");
}

void NPC::AddSpeech(const char* text)
{
    speechs.push_back(text);
}

void NPC::Interact()
{
    if (associated.box.GetCenter().GetDistance(GameData::playerPos) > associated.box.w/2 + associated.box.h/2 + 25)
    {
        dialog->Close();
        talking = false;
        currentSpeech = 0;
    }
    else if (InputManager::GetInstance().KeyPress(E_KEY))
    {
        if (++currentSpeech == speechs.size())
        {
            dialog->Close();
            currentSpeech = 0;
            talking = false;
        } 
        else 
            dialog->SetText(speechs[currentSpeech]);
    }
}
