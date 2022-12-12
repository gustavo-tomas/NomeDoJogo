#include "../header/PenguinBody.h"
#include "../header/Sprite.h"
#include "../header/PenguinCannon.h"
#include "../header/Game.h"
#include "../header/InputManager.h"
#include "../header/Collider.h"
#include "../header/Bullet.h"
#include "../header/Camera.h"
#include "../header/Sound.h"

PenguinBody* PenguinBody::player;

PenguinBody::PenguinBody(GameObject& associated) : Component(associated)
{
    speed = Vec2(0, 0);
    linearSpeed = 0;
    angle = 0;
    hp = 100;
    
    Sprite* sprite = new Sprite(associated, "./assets/image/penguin.png");
    associated.AddComponent(sprite);

    Collider* collider = new Collider(associated);
    associated.AddComponent(collider);

    player = this;
}

PenguinBody::~PenguinBody()
{
    player = nullptr;
}

void PenguinBody::Start()
{
    State& state = Game::GetInstance().GetCurrentState();
    
    GameObject* pcannonGo = new GameObject();
    PenguinCannon* pCannon = new PenguinCannon(*pcannonGo, state.GetObjectPtr(&associated));
    
    pcannonGo->box.SetVec(associated.box.GetCenter() - pcannonGo->box.GetCenter());
    pcannonGo->AddComponent(pCannon);

    weak_ptr<GameObject> pcannonPtr = state.AddObject(pcannonGo);
    this->pcannon = pcannonPtr;
}

void PenguinBody::Update(float dt)
{
    if (hp <= 0)
    {
        associated.RequestDelete();
        if (!pcannon.expired()) pcannon.lock()->RequestDelete();
        return;
    }

    // Shoot
    if (InputManager::GetInstance().IsMouseDown(LEFT_MOUSE_BUTTON) && !pcannon.expired())
    {
        PenguinCannon* pCannon = (PenguinCannon*) pcannon.lock()->GetComponent("PenguinCannon");
        pCannon->Shoot();
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

    // Updates sprite and position
    associated.angleDeg = angle;
    speed = Vec2(linearSpeed, 0).GetRotated(angle);
    associated.box.SetVec(associated.box.GetVec() + speed * dt);
    
    // Limits X & Y position to the limits of the tilemap
    associated.box.x = min(1408.f - associated.box.w, associated.box.x);
    associated.box.x = max(0.f, associated.box.x);
    associated.box.y = min(1280.f - associated.box.h, associated.box.y);
    associated.box.y = max(0.f, associated.box.y);
}

void PenguinBody::Render()
{

}

bool PenguinBody::Is(const char* type)
{
    string str_type = type;
    return str_type == "PenguinBody";
}

void PenguinBody::NotifyCollision(GameObject& other)
{
    Bullet* bullet = (Bullet*) other.GetComponent("Bullet");
    if (bullet != nullptr && bullet->targetsPlayer)
    {
        hp -= bullet->GetDamage();
        if (hp <= 0)
        {
            Camera::Unfollow();

            GameObject* penguinDeathGo = new GameObject();
            penguinDeathGo->box = associated.box;

            Sprite* penguinDeathSprite = new Sprite(*penguinDeathGo, "./assets/image/penguindeath.png", 5, 0.25, 1.25);
            penguinDeathGo->AddComponent(penguinDeathSprite);
            
            Sound* penguinDeathSound = new Sound(*penguinDeathGo, "./assets/audio/boom.wav");
            penguinDeathSound->Play();
            penguinDeathGo->AddComponent(penguinDeathSound);

            Game::GetInstance().GetCurrentState().AddObject(penguinDeathGo);
        }
        cout << "PENGUIN HP: " << hp << endl;
    }
}