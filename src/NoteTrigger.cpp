#include "../header/NoteTrigger.h"
#include "../header/Collider.h"
#include "../header/Note.h"
#include "../header/Sprite.h"
#include "../header/InputManager.h"
#include "../header/Player.h"

NoteTrigger::NoteTrigger(GameObject& associated, int triggerKey, int32_t spriteNum) : Component(associated)
{
    this->triggerKey = triggerKey;

    Sprite *sprite = nullptr;
    if(spriteNum == 0)
        sprite = new Sprite(associated, "assets/image/icons/note1.png");

    else if(spriteNum == 1)
        sprite = new Sprite(associated, "assets/image/icons/note2.png");

    else if(spriteNum == 2)
        sprite = new Sprite(associated, "assets/image/icons/note3.png");

    else
        sprite = new Sprite(associated, "assets/image/icons/note4.png");

    sprite->SetScale(20.0 / sprite->GetWidth(), 20.0 / sprite->GetHeight());  
    associated.AddComponent(sprite);
    
    associated.box.h = associated.box.w = 20;

    Collider *collider = new Collider(associated, Vec2(1, 1), Vec2(0, 0), false);   
    associated.AddComponent(collider);
}

NoteTrigger::~NoteTrigger()
{
    
}

void NoteTrigger::Update(float dt)
{
    
}

void NoteTrigger::Render()
{
    
}

bool NoteTrigger::Is(const char* type)
{
    string str_type = type;
    return str_type == "NoteTrigger";
}

void NoteTrigger::NotifyCollision(GameObject& other)
{
    auto component = (Note*) other.GetComponent("Note");
    if (component != nullptr)
    {
        if (InputManager::GetInstance().KeyPress(triggerKey))
        {
            float auxValue = min(abs((associated.box.x + associated.box.w) - other.box.x), abs((other.box.x + other.box.w) - associated.box.x));
            float percentage = ceil(auxValue / 5) * 25;
            
            other.RequestDelete();

            Player::player->AddMana(10);
            Player::player->AddAttackPower(percentage);
        }
    }
}
