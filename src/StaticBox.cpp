#include "../header/StaticBox.h"
#include "../header/SpriteRect.h"
#include "../header/Collider.h"
#include "../header/Camera.h"

StaticBox::StaticBox(GameObject& associated, Vec2 position) : Component(associated)
{
    SpriteRect* spriteRect = new SpriteRect(associated, 0x88888888, 100, 100);
    associated.AddComponent(spriteRect);
    
    associated.box.SetVec(position + Camera::pos);
    lastCameraPos = Camera::pos;
}

void StaticBox::Start()
{
    
}

StaticBox::~StaticBox()
{
    
}

void StaticBox::NotifyCollision(GameObject& other)
{

}

void StaticBox::Update(float dt)
{
    associated.box.SetVec(associated.box.GetVec() - lastCameraPos + Camera::pos);
    lastCameraPos = Camera::pos;
}

void StaticBox::Render()
{

}

bool StaticBox::Is(const char* type)
{
    string str_type = type;
    return str_type == "TestBox";
}