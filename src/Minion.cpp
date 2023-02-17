#include "../header/Minion.h"
#include "../header/Sprite.h"
#include "../header/Bullet.h"
#include "../header/Game.h"
#include "../header/Collider.h"
#include "../header/UserInterface.h"
#include "../header/GameData.h"
#include "../header/StageState.h"

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
}

void Minion::Start()
{
    hp = 100;

    UserInterface* ui = new UserInterface(associated, Vec2(GameData::WIDTH - 75, 25), true);
    associated.AddComponent(ui);
    moving = false;
}

void Minion::Update(float dt)
{
    if (hp <= 0)
    {
        associated.RequestDelete();
        return;
    }

    shootTimer.Update(dt);
    if (shootTimer.Get() >= 1.2 && !StageState::playerTurn)
        Shoot(GameData::playerPos);
    
    moveTimer.Update(dt);

    if (moveTimer.Get() >= 0.5){
        Collider* collider = (Collider*) associated.GetComponent("Collider");
        float speed = 200.0;

        Vec2 minionPos = collider->box.GetCenter();

        if(!moving){
            velocity = Vec2(0.f, 1.f);
            destination = minionPos + Vec2(0, (rand() % 200) - 100);
            moving = true;

            if (destination.y <= GameData::HEIGHT / 5.0)
                destination.y = GameData::HEIGHT / 5.0;

            if (destination.y >= GameData::HEIGHT - GameData::HEIGHT / 3.0)
                destination.y = GameData::HEIGHT - GameData::HEIGHT / 3.0;

            if(destination.y < minionPos.y){
                velocity.y = -1.f;
            }
        }

        if (collider != nullptr && (velocity.x != 0.f || velocity.y != 0.f))
            collider->velocity = velocity.GetNormalized() * speed;
        else if(collider != nullptr)
            collider->velocity = {0, 0};
            
        float dist = minionPos.GetDistance(destination);
        Vec2 deltaS = (collider->velocity * dt);

        if (dist < deltaS.GetMagnitude())
        {
            collider->box.SetCenter(destination); 
            moveTimer.Restart();
            collider->velocity = {0, 0};
            moving = false;
        }
    }
    

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

        if (hp < 10) return;

        auto ui = (UserInterface *) associated.GetComponent("UserInterface");
        if (ui != nullptr)
            ui->UpdateLifebar(hp / 10);
    }
}

void Minion::Shoot(Vec2 pos)
{
    float angle = associated.box.GetCenter().GetAngle(pos) - (M_PI / 4.0);
    float speed = 300;
    float damage = 10;
    float maxDistance = 1000;

    GameObject* bulletGo = new GameObject();
    Vec2 center = associated.box.GetCenter();
    Vec2 offset = Vec2(-associated.box.w - 10, -bulletGo->box.h / 2.0);
    bulletGo->box.SetVec(center + offset);
    
    Bullet* bullet = new Bullet(*bulletGo, angle, speed, damage, maxDistance, "./assets/image/icons/note4.png");
    bulletGo->AddComponent(bullet);

    Game::GetInstance().GetCurrentState().AddObject(bulletGo,10020);
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
