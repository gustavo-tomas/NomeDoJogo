#include "../header/Minion.h"
#include "../header/Sprite.h"
// #include "../header/Bullet.h"
#include "../header/Game.h"
#include "../header/Collider.h"

Minion::Minion(GameObject& associated, Vec2 initialPos) : Component(associated)
{
    associated.box.SetVec(initialPos);

    Sprite* sprite = new Sprite(associated, "./assets/image/minion-45x66.png", 3, 2);
    associated.AddComponent(sprite);

    Collider* collider = new Collider(associated);
    associated.AddComponent(collider);
}

void Minion::Update(float dt)
{
}

void Minion::NotifyCollision(GameObject& other)
{

}

void Minion::Shoot(Vec2 pos)
{
    // float angle = associated.box.GetCenter().GetAngle(pos) - (M_PI / 4.0);
    // float speed = 250;
    // float damage = 10;
    // float maxDistance = 1000;

    GameObject* bulletGo = new GameObject();
    // Bullet* bullet = new Bullet(*bulletGo, angle, speed, damage, maxDistance, "./assets/image/minionbullet2.png", 3, 0.5);
    
    Vec2 center = associated.box.GetCenter();
    Vec2 offset = Vec2(bulletGo->box.w / 2.0, bulletGo->box.h / 2.0);
    
    bulletGo->box.SetVec(center - offset);
    // bulletGo->AddComponent(bullet);

    State& state = Game::GetInstance().GetCurrentState();
    state.AddObject(bulletGo);
}

void Minion::Render()
{

}

bool Minion::Is(const char* type)
{
    string str_type = type;
    return str_type == "Minion";
}
