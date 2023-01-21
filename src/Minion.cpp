#include "../header/Minion.h"
#include "../header/Sprite.h"
#include "../header/Bullet.h"
#include "../header/Game.h"
#include "../header/Collider.h"
#include "../header/Camera.h"
#include "../header/GameData.h"

Minion::Minion(GameObject& associated, Vec2 initialPos) : Component(associated)
{
    associated.box.SetVec(initialPos);

    Sprite* sprite = new Sprite(associated, "./assets/image/minion-45x66.png", 3);
    associated.AddComponent(sprite);

    Collider* collider = new Collider(associated);
    associated.AddComponent(collider);
}

void Minion::Start()
{
    hp = 100;
}

void Minion::Update(float dt)
{
    shootTimer.Update(dt);
    if (shootTimer.Get() >= 1.2)
        Shoot(GameData::playerPos);
}

void Minion::NotifyCollision(GameObject& other)
{
    if (associated.IsDead())
        return;

    Bullet* bullet = (Bullet*) other.GetComponent("Bullet");
    if (bullet != nullptr)
        hp -= bullet->GetDamage();

    if (hp <= 0)
        associated.RequestDelete();

    cout << "HP: " << hp << endl;
}

void Minion::Shoot(Vec2 pos)
{
    float angle = associated.box.GetCenter().GetAngle(pos) - (M_PI / 4.0);
    float speed = 150;
    float damage = 10;
    float maxDistance = 1000;

    GameObject* bulletGo = new GameObject();
    Bullet* bullet = new Bullet(*bulletGo, angle, speed, damage, maxDistance, "./assets/image/mage-bullet-13x13.png", 5, 0.5);
    
    Vec2 center = associated.box.GetCenter();
    Vec2 offset = Vec2(-associated.box.w - 10, -bulletGo->box.h / 2.0);
    
    bulletGo->box.SetVec(center + offset);
    bulletGo->AddComponent(bullet);

    Game::GetInstance().GetCurrentState().AddObject(bulletGo);
    shootTimer.Restart();
}

void Minion::Render()
{

}

bool Minion::Is(const char* type)
{
    string str_type = type;
    return str_type == "Minion";
}
