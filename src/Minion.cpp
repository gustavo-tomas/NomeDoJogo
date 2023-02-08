#include "../header/Minion.h"
#include "../header/Sprite.h"
#include "../header/Bullet.h"
#include "../header/Game.h"
#include "../header/CameraFollower.h"
#include "../header/Collider.h"
#include "../header/Camera.h"
#include "../header/GameData.h"

int Minion::minionCount = 0;

Minion::Minion(GameObject& associated, Vec2 initialPos) : Component(associated)
{
    associated.box.SetVec(initialPos);

    Sprite* sprite = new Sprite(associated, "./assets/image/minion-45x66.png", 3, 2);
    associated.AddComponent(sprite);

    Collider* collider = new Collider(associated);
    associated.AddComponent(collider);

    Minion::minionCount++;
}

Minion::~Minion()
{
    Minion::minionCount--;
    lives.clear();
}

void Minion::Start()
{
    hp = 100;

    State& state = Game::GetInstance().GetCurrentState();
    for (int i = 0; i * 10 < hp; i++)
    {
        GameObject* heartGo = new GameObject();
        CameraFollower* cameraFollower = new CameraFollower(*heartGo, {(float) GameData::WIDTH - 35 - i * 24, 20});
        Sprite* sprite = new Sprite(*heartGo, "./assets/image/hearty_strip6.png", 6, 1);
        sprite->SetScale(0.5, 0.5);

        heartGo->AddComponent(sprite);
        heartGo->AddComponent(cameraFollower);
        lives.push_back(state.AddObject(heartGo));
    }
}

void Minion::Update(float dt)
{
    if (hp <= 0)
    {
        associated.RequestDelete();
        return;
    }

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
    {
        int bulletDamage = bullet->GetDamage();
        hp -= bulletDamage;
        if (!lives.empty())
        {
            for (int i = 0; i * 10 < bulletDamage && !lives.empty(); i++)
            {
                lives[lives.size() - 1].lock().get()->RequestDelete();
                lives.erase(lives.begin() + lives.size() - 1);
            }
        }
    }

    if (hp <= 0)
        associated.RequestDelete();
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
