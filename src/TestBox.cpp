#include "../header/TestBox.h"
#include "../header/Sprite.h"
#include "../header/Collider.h"

TestBox::TestBox(GameObject& associated, Vec2 position) : Component(associated)
{
    Sprite* sprite = new Sprite(associated, "./assets/image/box.png");
    associated.AddComponent(sprite);
    
    Collider* collider = new Collider(associated);
    associated.AddComponent(collider);

    associated.box.SetVec(position);
}

void TestBox::Start()
{
    
}

TestBox::~TestBox()
{
    
}

void TestBox::NotifyCollision(GameObject& other)
{

}

void TestBox::Update(float dt)
{

}

void TestBox::Render()
{

}

bool TestBox::Is(const char* type)
{
    string str_type = type;
    return str_type == "TestBox";
}