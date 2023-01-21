#include "../header/Bullet.h"
#include "../header/Sprite.h"
#include "../header/Collider.h"
#include "../header/Minion.h"
#include <cmath>

Bullet::Bullet(GameObject& associated, float angle, float speed,
    int damage, float maxDistance, const char* sprite,
    int frameCount, float frameTime, bool targetsPlayer)
    : Component(associated)
{
    Sprite* bulletSprite = new Sprite(associated, sprite, frameCount, 1, frameTime);
    bulletSprite->SetScale(2.0, 2.0);
    associated.AddComponent(bulletSprite);

    Collider* collider = new Collider(associated);
    collider->kinematic = true;
    associated.AddComponent(collider);

    // associated.angleDeg = angle;

    this->targetsPlayer = targetsPlayer;
    this->speed = Vec2(speed, speed).GetRotated(angle);
    this->damage = damage;
    this->distanceLeft = maxDistance;
}

void Bullet::Update(float dt)
{
    if (distanceLeft <= 0)
    {
        cout << "Bullet deleted" << endl;
        associated.RequestDelete();
        return;
    }
    
    Vec2 deltaS = speed * dt;
    associated.box.SetVec(associated.box.GetVec() + deltaS);
    distanceLeft -= Vec2(deltaS.x, deltaS.y).GetMagnitude();
}

void Bullet::Render()
{

}

bool Bullet::Is(const char* type)
{
    string str_type = type;
    return str_type == "Bullet";
}

void Bullet::NotifyCollision(GameObject& other)
{
    if (!associated.IsDead())
        associated.RequestDelete();
}

int Bullet::GetDamage()
{
    return damage;
}
