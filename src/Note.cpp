#include "../header/Note.h"
#include "../header/Collider.h"
#include "../header/SpriteRect.h"

Note::Note(GameObject& associated) : Component(associated)
{
    this->speed = 1;
}

Note::Note(GameObject& associated, float speed) : Component(associated)
{
    this->speed = speed;
    associated.box.h = associated.box.w = 18;

    Collider *collider = new Collider(associated);   
    associated.AddComponent(collider);

    // SpriteRect* sr = new SpriteRect(associated, 0xFF3333FF ^ random(), 18, 18);
    // associated.AddComponent(sr);
}

Note::~Note()
{
    
}

void Note::Update(float dt)
{
    associated.box.x -= speed * dt;
    if(associated.box.x < 0){
        associated.RequestDelete();
    }
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
