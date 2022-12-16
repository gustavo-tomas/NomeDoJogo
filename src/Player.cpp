#include "../header/Player.h"
#include "../header/Sprite.h"
#include "../header/Game.h"
#include "../header/InputManager.h"
#include "../header/Collider.h"
#include "../header/Camera.h"
#include "../header/Sound.h"

Player* Player::player;

Player::Player(GameObject& associated) : Component(associated)
{
    linearSpeed = 0;
    angle = 0;
    hp = 100;
    
    Sprite* sprite = new Sprite(associated, "./assets/image/box.png");
    sprite->SetScale(0.3, 0.3);
    associated.AddComponent(sprite);

    Collider* collider = new Collider(associated);
    collider->SetMass(1.0);

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

    float acc = 500.0;
    float dec = 300.0;
    float maxSpeed = 550.0;

    // Accelerates
    if (InputManager::GetInstance().IsKeyDown(W_KEY))
        linearSpeed = linearSpeed < maxSpeed ? linearSpeed + acc * dt : maxSpeed;

    // Decelerates
    if (InputManager::GetInstance().IsKeyDown(S_KEY))
        linearSpeed = linearSpeed > -maxSpeed ? linearSpeed - acc * dt : -maxSpeed;

    // Turns Right
    if (InputManager::GetInstance().IsKeyDown(D_KEY))
        angle += 2.5 * dt;

    // Turns Left
    if (InputManager::GetInstance().IsKeyDown(A_KEY))
        angle -= 2.5 * dt;

    // Comes to a halt
    if (!InputManager::GetInstance().IsKeyDown(W_KEY) &&
        !InputManager::GetInstance().IsKeyDown(S_KEY))
    {
        linearSpeed = linearSpeed > 0 ? linearSpeed - dec * dt : linearSpeed;
        linearSpeed = linearSpeed < 0 ? linearSpeed + dec * dt : linearSpeed;
        linearSpeed = abs(linearSpeed) <= 5.0 ? 0 : linearSpeed - 1.0;
    }

    // @TODO: atualizar esses dados no collider?
    // Updates sprite and velocity
    Collider* collider = (Collider*) associated.GetComponent("Collider");
    associated.angleDeg = angle;
    collider->velocity = Vec2(linearSpeed, 0).GetRotated(angle);
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