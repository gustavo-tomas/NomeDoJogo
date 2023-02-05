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

#define ATTACK_ANIMATION_DURATION 0.25
#define KNOCKBACK_DURATION 1

Player* Player::player;
const Player::SpriteInfo files[7] = { 
    {"./assets/image/player/Magic_Girl_Idle.png", 8, 1},        // IDLE
    {"./assets/image/player/Magic_Girl_Walk_Left.png", 6, 1},   // LEFT
    {"./assets/image/player/Magic_Girl_Walk_Up.png", 8, 1},     // UP
    {"./assets/image/player/Magic_Girl_Walk_Right.png", 6, 1},  // RIGHT
    {"./assets/image/player/Magic_Girl_Walk_Down.png", 8, 1},   // DOWN
    {"./assets/image/player/Magic_Girl_Idle.png", 8, 1},        // ATTACK
    {"./assets/image/player/Magic_Girl_Idle.png", 8, 1}         // DAMAGED
};

Player::Player(GameObject& associated, bool moveLimits) : Component(associated)
{
    linearSpeed = 0;
    angle = 0;
    hp = 100;
    mana = 0;
    attackPower = 0;
    currentAction = previousAction = Action::IDLE;

    this->moveLimits = moveLimits;
    Sprite* sprite = new Sprite(
        associated, files[Action::IDLE].fileName, files[Action::IDLE].frameCountX, files[Action::IDLE].frameCountY, 0.2
    );
    associated.AddComponent(sprite);

    Collider* collider = new Collider(associated);
    // Collider* collider = new Collider(associated, Vec2(0.70, 0.35), Vec2(0, 25));
    // Collider* collider = new Collider(associated, Vec2(0.70, 0.35), Vec2(0, 25), false);
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
    // float speed = 900.0; // For tests
    Vec2 velocity = Vec2(0.f, 0.f);

    // Up
    if (InputManager::GetInstance().IsKeyDown(W_KEY) && (!moveLimits || associated.box.y > GameData::HEIGHT/5)) 
        velocity.y -= 1.f;

    // Down
    if (InputManager::GetInstance().IsKeyDown(S_KEY) && (!moveLimits || associated.box.y + associated.box.h < GameData::HEIGHT - GameData::HEIGHT/3))
        velocity.y += 1.f;

    // Right
    if (InputManager::GetInstance().IsKeyDown(D_KEY) && (!moveLimits || associated.box.x + associated.box.w < GameData::WIDTH/3)) 
        velocity.x += 1.f;

    // Left
    if (InputManager::GetInstance().IsKeyDown(A_KEY) && (!moveLimits || associated.box.x > GameData::WIDTH/20))
        velocity.x -= 1.f;

    if(currentAction != previousAction){
        Vec2 currentPos = associated.box.GetCenter();
        ((Sprite *) associated.GetComponent("Sprite"))->ChangeSprite(
            files[currentAction].fileName, files[currentAction].frameCountX, files[currentAction].frameCountY, 0.2
        );
        associated.box.SetCenter(currentPos);
    }

    switch(currentAction)
    {
    case Action::IDLE:
        ActionsHandler(velocity);

        previousAction = Action::IDLE;
        break;

    case Action::WALKING_UP:            
        ActionsHandler(velocity);

        previousAction = Action::WALKING_UP;
        break;

    case Action::WALKING_DOWN:      
        ActionsHandler(velocity);

        previousAction = Action::WALKING_DOWN;
        break;

    case Action::WALKING_LEFT:            
        ActionsHandler(velocity);

        previousAction = Action::WALKING_LEFT;
        break;

    case Action::WALKING_RIGHT:            
        ActionsHandler(velocity);

        previousAction = Action::WALKING_RIGHT;
        break;

    case Action::ATTACKING:
        actionTimer.Update(dt);

        if(actionTimer.Get() > ATTACK_ANIMATION_DURATION){
            Shoot();
            ActionsHandler(velocity);
            actionTimer.Restart();
        }

        previousAction = Action::ATTACKING;
        break;

    case Action::TAKING_DAMAGE:
        actionTimer.Update(dt);

        velocity = { 0, 0 }; // Can't move when hit
        if(actionTimer.Get() > KNOCKBACK_DURATION){
            ActionsHandler(velocity);
            actionTimer.Restart();
        }

        previousAction = Action::TAKING_DAMAGE;
        break;
    }

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

void Player::ActionsHandler(Vec2 velocity){
    if(InputManager::GetInstance().IsKeyDown(SPACE_KEY) && mana >= 20){
        currentAction = Action::ATTACKING;
        actionTimer.Restart();
    }

    else if(velocity.x > 0)
        currentAction = Action::WALKING_RIGHT;
    
    else if(velocity.x < 0)
        currentAction = Action::WALKING_LEFT;
    
    else if(velocity.y > 0)
        currentAction = Action::WALKING_DOWN;
    
    else if(velocity.y < 0)
        currentAction = Action::WALKING_UP;    

    else
        currentAction = Action::IDLE;
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
    if (bullet != nullptr){
        hp -= bullet->GetDamage();
        currentAction = Action::TAKING_DAMAGE;
        actionTimer.Restart();
    }
}
