#include "../header/Player.h"
#include "../header/Sprite.h"
#include "../header/Game.h"
#include "../header/InputManager.h"
#include "../header/Collider.h"
#include "../header/Collision.h"
#include "../header/Camera.h"
#include "../header/Sound.h"
#include "../header/Bullet.h"
#include "../header/GameData.h"

Player* Player::player;

Player::Player(GameObject& associated) : Component(associated)
{
    linearSpeed = 0;
    angle = 0;
    hp = 100;
    
    Sprite* sprite = new Sprite(associated, "./assets/image/mage-1-85x94.png", 4);
    associated.AddComponent(sprite);

    Collider* collider = new Collider(associated);
    associated.AddComponent(collider);

    player = this;
}

Player::~Player()
{
    player = nullptr;
}

void Player::Start()
{
    
}

void Player::Update(float dt)
{
    if (hp <= 0)
    {
        associated.RequestDelete();
        return;
    }

    float speed = 300.0;
    Vec2 velocity = Vec2(0.f, 0.f);

    // Up
    if (InputManager::GetInstance().IsKeyDown(W_KEY)) 
        velocity.y -= 1.f;

    // Down
    if (InputManager::GetInstance().IsKeyDown(S_KEY))
        velocity.y += 1.f;

    // Right
    if (InputManager::GetInstance().IsKeyDown(D_KEY)) 
        velocity.x += 1.f;

    // Left
    if (InputManager::GetInstance().IsKeyDown(A_KEY))
        velocity.x -= 1.f;

    // Shoot
    if (InputManager::GetInstance().IsKeyDown(SPACE_KEY))
        Shoot();

    // Updates shoot timer
    shootTimer.Update(dt);

    Collider* collider = (Collider*) associated.GetComponent("Collider");
    if (collider != nullptr && (velocity.x != 0.f || velocity.y != 0.f))
        collider->velocity = velocity.GetNormalized() * speed;

    else
        collider->velocity = velocity;

    GameData::playerPos = associated.box.GetCenter();
}

void Player::Shoot()
{
    if (shootTimer.Get() < 0.40)
        return;

    float speed = 750;
    float damage = 20;
    float maxDistance = 1000;

    GameObject* bulletGo = new GameObject();
    Bullet* bullet = new Bullet(*bulletGo, angle - (M_PI / 4.0), speed, damage, maxDistance, "./assets/image/mage-bullet-13x13.png", 5, 0.5, false);
    
    Vec2 center = associated.box.GetCenter();
    Vec2 offset = Vec2(associated.box.w, -bulletGo->box.h / 2.0);

    bulletGo->box.SetVec(center + offset);
    bulletGo->AddComponent(bullet);

    Game::GetInstance().GetCurrentState().AddObject(bulletGo);
    shootTimer.Restart();
}

void Player::Render()
{

}

bool Player::Is(const char* type)
{
    string str_type = type;
    return str_type == "Player";
}

void Player::NotifyCollision(GameObject& other)
{
    
}