#include "../header/Player.h"
#include "../header/Sprite.h"
#include "../header/Game.h"
#include "../header/InputManager.h"
#include "../header/Collider.h"
#include "../header/Collision.h"
#include "../header/Camera.h"
#include "../header/Sound.h"

Player* Player::player;

Player::Player(GameObject& associated) : Component(associated)
{
    linearSpeed = 0;
    angle = 0;
    hp = 100;
    
    Sprite* sprite = new Sprite(associated, "./assets/image/250_scout.png");
    sprite->SetScale(0.3, 0.3);
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

    float speed = 200.0;
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

    Collider* collider = (Collider*) associated.GetComponent("Collider");
    if (collider != nullptr && (velocity.x != 0.f || velocity.y != 0.f))
        collider->velocity = velocity.GetNormalized() * speed;

    else
        collider->velocity = velocity;
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