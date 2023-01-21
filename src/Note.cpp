#include "../header/Note.h"
#include "../header/SpriteRect.h"

Note::Note(GameObject& associated, float speed) : Component(associated)
{
    this->speed = speed;
    SpriteRect* sr = new SpriteRect(associated, 0xFF3333FF ^ random(), 20, 20);
    associated.AddComponent(sr);
}

void Note::Update(float dt)
{
    // associated.box.x -= speed * dt;
    associated.box.x -= speed;
}

void Note::Render()
{
    
}

bool Note::Is(const char* type)
{
    
}

void Note::NotifyCollision(GameObject& other)
{
    
}
