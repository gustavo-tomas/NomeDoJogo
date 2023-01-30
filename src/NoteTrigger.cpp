#include "../header/NoteTrigger.h"
#include "../header/Collider.h"
#include "../header/Note.h"
#include "../header/SpriteRect.h"
#include "../header/InputManager.h"
#include "../header/Player.h"

NoteTrigger::NoteTrigger(GameObject& associated, int triggerKey) : Component(associated)
{
    associated.box.h = associated.box.w = 20;
    this->triggerKey = triggerKey;

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
