#include "../header/Note.h"
#include "../header/SpriteRect.h"

Note::Note(GameObject& associated) : Component(associated)
{
    SpriteRect* sr = new SpriteRect(associated, 0xFF3333FF, 50, 50);
    associated.AddComponent(sr);
}

void Note::Update(float dt)
{
    
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
