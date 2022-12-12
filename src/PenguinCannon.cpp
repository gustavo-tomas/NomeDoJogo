#include "../header/PenguinCannon.h"
#include "../header/Sprite.h"
#include "../header/InputManager.h"
#include "../header/Camera.h"
#include "../header/Bullet.h"
#include "../header/StageState.h"
#include "../header/Game.h"
#include "../header/Collider.h"

PenguinCannon::PenguinCannon(GameObject& associated, weak_ptr<GameObject> penguinBody) : Component(associated)
{
    pbody = penguinBody;
    angle = 0;

    Sprite* sprite = new Sprite(associated, "./assets/image/cubngun.png");
    associated.AddComponent(sprite);

    Collider* collider = new Collider(associated, Vec2(0.5, 1), Vec2(-associated.box.w / 4.0, 0));
    associated.AddComponent(collider);
}

void PenguinCannon::Update(float dt)
{
    if (pbody.expired())
    {
        associated.RequestDelete();
        return;
    }
    
    // Updates position
    if (!pbody.expired())
        associated.box.SetVec(pbody.lock()->box.GetCenter() - Vec2(associated.box.w / 2, associated.box.h / 2));

    // Updates rotation
    Vec2 pos = Vec2(InputManager::GetInstance().GetMouseX(), InputManager::GetInstance().GetMouseY()) + Camera::pos;
    angle = associated.box.GetCenter().GetAngle(pos);
    associated.angleDeg = angle;

    // Updates timer
    timer.Update(dt);
}

void PenguinCannon::Render()
{

}

bool PenguinCannon::Is(const char* type)
{
    string str_type = type;
    return str_type == "PenguinCannon";
}

void PenguinCannon::NotifyCollision(GameObject& other)
{
    if (!pbody.expired()) pbody.lock()->NotifyCollision(other);
}

void PenguinCannon::Shoot()
{
    if (timer.Get() < 0.40)
        return;

    float speed = 700;
    float damage = 10;
    float maxDistance = 1000;

    GameObject* bulletGo = new GameObject();
    Bullet* bullet = new Bullet(*bulletGo, angle - (M_PI / 4.0), speed, damage, maxDistance, "./assets/image/penguinbullet.png", 4, 0.5, false);
    
    Vec2 center = associated.box.GetCenter();
    Vec2 offset = Vec2(bulletGo->box.w / 2.0, bulletGo->box.h / 2.0) - Vec2(associated.box.w / 2.0, 0).GetRotated(angle);
    
    bulletGo->box.SetVec(center - offset);
    bulletGo->AddComponent(bullet);

    Game::GetInstance().GetCurrentState().AddObject(bulletGo);

    timer.Restart();
}