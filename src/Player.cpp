#include "../header/Player.h"
#include "../header/Sprite.h"
#include "../header/Game.h"
#include "../header/InputManager.h"
#include "../header/CameraFollower.h"
#include "../header/Collider.h"
#include "../header/Collision.h"
#include "../header/Camera.h"
#include "../header/Sound.h"
#include "../header/Bullet.h"
#include "../header/GameData.h"

Player* Player::player;

Player::Player(GameObject& associated, bool moveLimits) : Component(associated)
{
    linearSpeed = 0;
    angle = 0;
    hp = 100;
    mana = 0;
    attackPower = 0;
    
    this->moveLimits = moveLimits;
    Sprite* sprite = new Sprite(associated, "./assets/image/mage-1-85x94.png", 4, 2);
    associated.AddComponent(sprite);

    // Collider* collider = new Collider(associated);
    // Collider* collider = new Collider(associated, Vec2(0.70, 0.35), Vec2(0, 25));
    Collider* collider = new Collider(associated, Vec2(0.70, 0.35), Vec2(0, 25), false);
    associated.AddComponent(collider);

    player = this;
}

Player::~Player()
{
    player = nullptr;
    lives.clear();
}

void Player::Start()
{
    State& state = Game::GetInstance().GetCurrentState();

    for (int i = 0; i * 10 < hp; i++)
    {
        GameObject* heartGo = new GameObject();
        CameraFollower* cameraFollower = new CameraFollower(*heartGo, {(float) i * 22, 20});
        Sprite* sprite = new Sprite(*heartGo, "./assets/image/Heart.png");

        heartGo->AddComponent(sprite);
        heartGo->AddComponent(cameraFollower);
        lives.push_back(state.AddObject(heartGo));
    }
}

void Player::Update(float dt)
{
    if (hp <= 0)
    {
        associated.RequestDelete();
        return;
    }

    // float speed = 300.0;
    float speed = 900.0; // For tests
    Vec2 velocity = Vec2(0.f, 0.f);

    // Up
    if (InputManager::GetInstance().IsKeyDown(W_KEY) && (!moveLimits || associated.box.y > GameData::HEIGHT / 5.0)) 
        velocity.y -= 1.f;

    // Down
    if (InputManager::GetInstance().IsKeyDown(S_KEY) && (!moveLimits || associated.box.y + associated.box.h < GameData::HEIGHT - GameData::HEIGHT / 3.0))
        velocity.y += 1.f;

    // Right
    if (InputManager::GetInstance().IsKeyDown(D_KEY) && (!moveLimits || associated.box.x + associated.box.w < GameData::WIDTH / 3.0)) 
        velocity.x += 1.f;

    // Left
    if (InputManager::GetInstance().IsKeyDown(A_KEY) && (!moveLimits || associated.box.x > GameData::WIDTH / 20.0))
        velocity.x -= 1.f;

    // Shoot
    if (InputManager::GetInstance().IsKeyDown(SPACE_KEY) && mana > 0)
        Shoot();

    Collider* collider = (Collider*) associated.GetComponent("Collider");
    if (collider != nullptr && (velocity.x != 0.f || velocity.y != 0.f))
        collider->velocity = velocity.GetNormalized() * speed;

    else
        collider->velocity = velocity; // {0, 0}

    if (collider != nullptr)
        GameData::playerPos = collider->box.GetCenter();

    else
    {
        cout << "Player doesn't have a Collider, using associated position instead!\n";
        GameData::playerPos = associated.box.GetCenter();
    }
}

void Player::Shoot()
{
    float speed = 750;
    float maxDistance = 1000;

    GameObject* bulletGo = new GameObject();
    Bullet* bullet = new Bullet(*bulletGo, angle - (M_PI / 4.0),
                                            speed, attackPower, maxDistance,
                                    "./assets/image/mage-bullet-13x13.png",
                                5, 0.5, false,
                                "./assets/audio/papapa.mp3");
    
    Vec2 center = associated.box.GetCenter();
    Vec2 offset = Vec2(associated.box.w, -bulletGo->box.h / 2.0);

    bulletGo->box.SetVec(center + offset);
    bulletGo->AddComponent(bullet);

    Game::GetInstance().GetCurrentState().AddObject(bulletGo);

    Sound* shootSound = (Sound *) associated.GetComponent("Sound");
    if (shootSound != nullptr)
        shootSound->Play();

    ResetMana();
    ResetAttackPower();
}

void Player::ResetAttackPower() 
{
    attackPower = 0;
}

void Player::AddAttackPower(float value) 
{
    attackPower += value;
}

void Player::ResetMana() 
{
    mana = 0;
}

void Player::AddMana(int value) 
{
    mana += value;
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
    Bullet* bullet = (Bullet *) other.GetComponent("Bullet");
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
}
