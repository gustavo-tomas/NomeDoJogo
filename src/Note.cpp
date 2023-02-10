#include "../header/Note.h"
#include "../header/Collider.h"
#include "../header/Sprite.h"

Note::Note(GameObject& associated) : Component(associated)
{
    this->speed = 1;
}

Note::Note(GameObject& associated, float speed, int32_t spriteNum) : Component(associated)
{
    this->speed = speed;

    Sprite *sprite = new Sprite(associated, "assets/image/icons/note7.png");
    
    sprite->SetScale(18.0 / sprite->GetWidth(), 18.0 / sprite->GetHeight());  
    associated.AddComponent(sprite);
    
    associated.box.h = associated.box.w = 18;

    Collider *collider = new Collider(associated, Vec2(1, 1), Vec2(0, 0), false);  
    associated.AddComponent(collider);
}

Note::~Note()
{
    
}

void Note::Update(float dt)
{
    if (associated.box.x < 0)
    {
        associated.RequestDelete();
        return;
    }

    Collider* collider = (Collider*) associated.GetComponent("Collider");
    if (collider != nullptr)
        collider->velocity = -speed;
}

void Note::Render()
{
    
}

bool Note::Is(const char* type)
{
    string str_type = type;
    return str_type == "Note";
}

void Note::NotifyCollision(GameObject& other)
{
    
}
