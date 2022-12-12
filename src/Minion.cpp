#include "../header/Minion.h"
#include "../header/Sprite.h"
#include "../header/Bullet.h"
#include "../header/Game.h"
#include "../header/Collider.h"
#include <string>

Minion::Minion(GameObject& associated, weak_ptr<GameObject> alienCenter, float arcOffsetDeg) : Component(associated)
{
    Sprite* sprite = new Sprite(associated, "./assets/image/minion.png");
    associated.AddComponent(sprite);

    Collider* collider = new Collider(associated);
    associated.AddComponent(collider);
    
    float r = 1.0 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(1.5 - 1.0)));
    Vec2 scale = Vec2(r, r);
    sprite->SetScale(scale.x, scale.y);

    arc = arcOffsetDeg;
    if (!alienCenter.expired()) this->alienCenter = alienCenter.lock().get();
}

void Minion::Update(float dt)
{
    if (alienCenter == nullptr || alienCenter->IsDead())
    {
        associated.RequestDelete();
        return;
    }

    Vec2 rotated = Vec2(200, 0).GetRotated(arc);

    associated.angleDeg = arc;

    associated.box.x = alienCenter->box.GetCenter().x - associated.box.w / 2.0 + rotated.x;
    associated.box.y = alienCenter->box.GetCenter().y - associated.box.h / 2.0 + rotated.y;
    arc = arc <= 2.0 * M_PI ? arc + ANG_INC * dt : dt; // Prevent overflow of arc
}

void Minion::NotifyCollision(GameObject& other)
{

}

void Minion::Shoot(Vec2 pos)
{
    float angle = associated.box.GetCenter().GetAngle(pos) - (M_PI / 4.0);
    float speed = 250;
    float damage = 10;
    float maxDistance = 1000;

    GameObject* bulletGo = new GameObject();
    Bullet* bullet = new Bullet(*bulletGo, angle, speed, damage, maxDistance, "./assets/image/minionbullet2.png", 3, 0.5);
    
    Vec2 center = associated.box.GetCenter();
    Vec2 offset = Vec2(bulletGo->box.w / 2.0, bulletGo->box.h / 2.0);
    
    bulletGo->box.SetVec(center - offset);
    bulletGo->AddComponent(bullet);

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