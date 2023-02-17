#include "../header/Player.h"
#include "../header/Sprite.h"
#include "../header/Game.h"
#include "../header/InputManager.h"
#include "../header/Collider.h"
#include "../header/Collision.h"
#include "../header/Sound.h"
#include "../header/Bullet.h"
#include "../header/GameData.h"
#include "../header/UserInterface.h"
#include "../header/StageState.h"
#include <string>

#define LOSS_BATTLE_DURATION 1.28
#define PREPARING_DURATION 2.7
#define PRACTING_DURATION 7.5
#define MAX_MANA 40
#define MIN_MANA 10

Player* Player::player;
const Player::SpriteInfo files[10] = { 
    {"./assets/image/player/Luna_Idle.png", 24, 1},             // IDLE
    {"./assets/image/player/Luna_Walk_Left.png", 12, 1},        // LEFT
    {"./assets/image/player/Luna_Walk_Up.png", 12, 1},          // UP
    {"./assets/image/player/Luna_Walk_Right.png", 12, 1},       // RIGHT
    {"./assets/image/player/Luna_Walk_Down.png", 12, 1},        // DOWN
    {"./assets/image/player/Luna_Flute_Idle.png", 12, 2},       // IDLE_PERFORMING
    {"./assets/image/player/Luna_Flute_Walk.png", 15, 2, 1},    // WALK_PERFORMING
    {"./assets/image/player/Luna_Prepare.png", 16, 2, 1},       // PREPARING
    {"./assets/image/player/Luna_Death.png", 6, 3, 2},          // LOST
    {"./assets/image/player/Luna_Flute_Idle.png", 12, 2}        // PRACTING
};

const Vec2 playerScale(0.1, 0.1);

Player::Player(GameObject& associated, bool moveLimits) : Component(associated)
{
    linearSpeed = 0;
    angle = 0;
    hp = 100;
    mana = 10;
    attackPower = 0;
    stunHeat = 0;

    currentAction = previousAction = Action::IDLE;

    this->moveLimits = moveLimits;
    Sprite* sprite = new Sprite(
        associated, files[Action::IDLE].fileName, 
        files[Action::IDLE].frameCountX, files[Action::IDLE].frameCountY, 
        files[Action::IDLE].frameTime
    );
    sprite->SetScale(playerScale.x, playerScale.y);
    
    associated.AddComponent(sprite);

    Collider* collider = new Collider(associated, {0.4, 0.2}, {0, 35});
    associated.AddComponent(collider);

    // UI Elements
    UserInterface* ui = new UserInterface(associated, {25, 25});
    associated.AddComponent(ui);

    // SFX
    Sound* walkingSound = new Sound(associated, "./assets/audio/sfx/walking_concrete.mp3", MIX_MAX_VOLUME);
    associated.AddComponent(walkingSound);
    walkingSound->Play(1);
    walkingSound->Pause();

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
        currentAction = Action::LOSS;

    shootTimer.Update(dt);

    float speed = 300.0;
    // float speed = 900.0; // For tests
    Vec2 velocity = Vec2(0.f, 0.f);

    // Stunned
    if (stunHeat >= 30)
    {
        stunTimer.Update(dt);
        if (stunTimer.Get() >= 1.5)
        {
            stunHeat = 0;
            stunTimer.Restart();
        }
    }

    if (stunHeat < 30)
    {
        Collider* collider = (Collider *) associated.GetComponent("Collider");

        // Up
        if (InputManager::GetInstance().IsKeyDown(W_KEY) && (!moveLimits || collider->box.y > GameData::HEIGHT / 5.0)) 
            velocity.y -= 1.f;

        // Down
        if (InputManager::GetInstance().IsKeyDown(S_KEY) && (!moveLimits || collider->box.y + collider->box.h < GameData::HEIGHT - GameData::HEIGHT / 3.0))
            velocity.y += 1.f;

        // Right
        if (InputManager::GetInstance().IsKeyDown(D_KEY) && (!moveLimits || collider->box.x + collider->box.w < GameData::WIDTH / 3.0)) 
            velocity.x += 1.f;

        // Left
        if (InputManager::GetInstance().IsKeyDown(A_KEY) && (!moveLimits || collider->box.x > GameData::WIDTH / 20.0))
            velocity.x -= 1.f;

        // Walking SFX
        Sound* sound = (Sound *) associated.GetComponent("Sound");
        if (sound != nullptr)
        {
            if (currentAction != Action::PREPARING && (velocity.x != 0 || velocity.y != 0))
            {
                if (!sound->IsOpen())
                    sound->Play(-1);

                else if (sound->IsOpen())
                    sound->Resume();
            }

            else if (sound->IsOpen())
                sound->Pause();
        }
    }

    if (currentAction != previousAction)
    {
        Vec2 currentPos = associated.box.GetCenter();
        ((Sprite *) associated.GetComponent("Sprite"))->ChangeSprite(
            files[currentAction].fileName, files[currentAction].frameCountX,
             files[currentAction].frameCountY, files[currentAction].frameTime,
             files[currentAction].framesMissing
        );
        ((Sprite *) associated.GetComponent("Sprite"))->SetScale(playerScale.x, playerScale.y);

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

    case Action::WALK_PERFORMING:
        if (velocity.x == 0 && velocity.y == 0)
            currentAction = Action::IDLE_PERFORMING;

        if (InputManager::GetInstance().IsKeyDown(SPACE_KEY) && mana >= 20 && StageState::playerTurn)
            Shoot();
        
        previousAction = Action::WALK_PERFORMING;
        break;

    case Action::IDLE_PERFORMING:
        if (velocity.x != 0 || velocity.y != 0)
            currentAction = Action::WALK_PERFORMING;

        if (InputManager::GetInstance().IsKeyDown(SPACE_KEY) && mana >= 20 && StageState::playerTurn && shootTimer.Get() > 0.4)
            Shoot();
        
        previousAction = Action::IDLE_PERFORMING;
        break;

    case Action::PREPARING:
        actionTimer.Update(dt);
        velocity = {0, 0};

        if(actionTimer.Get() > PREPARING_DURATION){
            currentAction = Action::IDLE;
            Game::GetInstance().Push(new StageState());
            actionTimer.Restart();
        }

        previousAction = Action::PREPARING;
        break;

    case Action::LOSS:
        actionTimer.Update(dt);
        velocity = {0, 0};

        if(actionTimer.Get() > LOSS_BATTLE_DURATION){
            associated.RequestDelete();
            return;
        }

        previousAction = Action::LOSS;

        break;

    case Action::PRACTING:
        actionTimer.Update(dt);
        velocity = {0, 0};

        if(actionTimer.Get() > PRACTING_DURATION){
            currentAction = Action::IDLE;
            actionTimer.Restart();
        }

        previousAction = Action::PRACTING;
    }

    Collider* collider = (Collider*) associated.GetComponent("Collider");
    if (collider != nullptr && (velocity.x != 0.f || velocity.y != 0.f))
        collider->velocity = velocity.GetNormalized() * speed;

    else
        collider->velocity = {0, 0};

    if (collider != nullptr)
        GameData::playerPos = collider->box.GetCenter();

    else
    {
        cout << "Player doesn't have a Collider, using associated position instead!\n";
        GameData::playerPos = associated.box.GetCenter();
    }
}

void Player::ActionsHandler(Vec2 velocity)
{
    if (velocity.y > 0)
        currentAction = Action::WALKING_DOWN;

    else if (velocity.x > 0)
        currentAction = Action::WALKING_RIGHT;
    
    else if (velocity.x < 0)
        currentAction = Action::WALKING_LEFT;
    
    else if (velocity.y < 0)
        currentAction = Action::WALKING_UP;    

    else
        currentAction = Action::IDLE;
}

void Player::Shoot()
{
    shootTimer.Restart();

    float speed = 750;
    float maxDistance = 1000;

    GameObject* bulletGo = new GameObject();
    Vec2 center = associated.box.GetCenter();
    Vec2 offset = Vec2(associated.box.w, -associated.box.h / 4.0);
    bulletGo->box.SetCenter(center + offset);

    string sprite = GameData::imagesPath;
    string audio = GameData::audiosPath;

    if (mana == MAX_MANA)
    {
        sprite += "icons/note5.png";
        audio += "sfx/special_attack.mp3";
        ResetMana();
    }
    else
    {
        sprite += "icons/note1.png";
        audio += "sfx/attack.mp3";
        AddMana(-10);
    }

    Bullet* bullet = new Bullet(*bulletGo, angle - (M_PI / 4.0),
                                    speed, attackPower, maxDistance,
                                    sprite.c_str(), 1, 1, false,
                                    audio);
    
    bulletGo->AddComponent(bullet);

    Game::GetInstance().GetCurrentState().AddObject(bulletGo, 10020);

    if (mana == MIN_MANA)
        ResetAttackPower();

    else
        AddAttackPower(-5);
}

void Player::ResetAttackPower() 
{
    attackPower = 0;
}

void Player::AddAttackPower(float value) 
{
    attackPower += value;
    attackPower = max(0.f, attackPower);
}

void Player::RestoreHealth()
{
    hp = 100;
}

void Player::ResetMana() 
{
    mana = MIN_MANA;
    
    auto ui = (UserInterface *) associated.GetComponent("UserInterface");
    if (ui != nullptr)
        ui->UpdateManabar(mana / 10);
}

void Player::AddMana(int value) 
{
    mana = min(mana + value, MAX_MANA);
    mana = max(mana, MIN_MANA);

    auto ui = (UserInterface *) associated.GetComponent("UserInterface");
    if (ui != nullptr)
        ui->UpdateManabar(mana / 10);
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
        stunHeat += bulletDamage;

        if (hp <= 0) return;

        auto ui = (UserInterface *) associated.GetComponent("UserInterface");
        if (ui != nullptr)
            ui->UpdateLifebar(hp / 10);
    }
}

void Player::SetAction(Action action)
{
    currentAction = action;
}
