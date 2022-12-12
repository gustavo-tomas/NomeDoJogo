#include "../header/Bullet.h"
#include "../header/Sprite.h"
#include "../header/Collider.h"
#include "../header/PenguinBody.h"
#include "../header/PenguinCannon.h"
#include "../header/Minion.h"
#include "../header/Alien.h"

Bullet::Bullet(GameObject& associated, float angle, float speed,
    int damage, float maxDistance, const char* sprite,
    int frameCount, float frameTime, bool targetsPlayer)
    : Component(associated)
{
    Sprite* bulletSprite = new Sprite(associated, sprite, frameCount, frameTime);
    bulletSprite->SetScale(1.2, 1.8);
    associated.AddComponent(bulletSprite);

    Collider* collider = new Collider(associated);
    associated.AddComponent(collider);

    associated.angleDeg = (angle + (M_PI / 4.0));

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
    // Minion bullet hit a player
    if (targetsPlayer &&
        ((PenguinBody*) other.GetComponent("PenguinBody") != nullptr ||
        (PenguinCannon*) other.GetComponent("PenguinCannon") != nullptr))
    {
        distanceLeft = 0;
        associated.RequestDelete();
    }

    // Player bullet hit an Alien
    else if (!targetsPlayer &&
        (Alien*) other.GetComponent("Alien") != nullptr)
    {
        distanceLeft = 0;
        associated.RequestDelete();
    }
}

int Bullet::GetDamage()
{
    return damage;
}