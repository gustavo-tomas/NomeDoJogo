#include "../header/Alien.h"
#include "../header/Sprite.h"
#include "../header/InputManager.h"
#include "../header/Camera.h"
#include "../header/Minion.h"
#include "../header/Game.h"
#include "../header/Collider.h"
#include "../header/Bullet.h"
#include "../header/Sound.h"
#include "../header/PenguinBody.h"
#include "../header/GameData.h"

int Alien::alienCount = 0;

Alien::Alien(GameObject& associated, int nMinions, float timeOffset) : Component(associated)
{
    Sprite* sprite = new Sprite(associated, "./assets/image/alien.png");
    associated.AddComponent(sprite);
    
    Collider* collider = new Collider(associated);
    associated.AddComponent(collider);

    speed = Vec2(0, 0);
    hp = 20;
    alienCount++;
    state = AlienState::RESTING;
    this->timeOffset = timeOffset;
    this->nMinions = nMinions;
}

void Alien::Start()
{
    State& state = Game::GetInstance().GetCurrentState();
    auto alienPtr = state.GetObjectPtr(&associated); 
    float arcOffSet = (2.0 * M_PI) / nMinions;

    for (int i = 0; i < nMinions; i++)
    {
        GameObject* minionGo = new GameObject();
        Minion* minion = new Minion(*minionGo, alienPtr, arcOffSet * (float) i);

        minionGo->AddComponent(minion);
        auto minionPtr = state.AddObject(minionGo);
        minionArray.push_back(minionPtr);
    }
}

Alien::~Alien()
{
    alienCount--;
    minionArray.clear();
}

void Alien::NotifyCollision(GameObject& other)
{
    Bullet* bullet = (Bullet*) other.GetComponent("Bullet");
    if (hp > 0 && bullet != nullptr && !bullet->targetsPlayer)
    {
        hp -= bullet->GetDamage();
        if (hp <= 0)
        {
            GameObject* alienDeathGo = new GameObject();
            alienDeathGo->box = associated.box;

            Sprite* alienDeathSprite = new Sprite(*alienDeathGo, "./assets/image/aliendeath.png", 4, 0.25, 1.0);
            alienDeathGo->AddComponent(alienDeathSprite);
            
            Sound* alienDeathSound = new Sound(*alienDeathGo, "./assets/audio/boom.wav");
            alienDeathSound->Play();
            alienDeathGo->AddComponent(alienDeathSound);

            Game::GetInstance().GetCurrentState().AddObject(alienDeathGo);
        }
    }
}

void Alien::Update(float dt)
{
    if (hp <= 0)
    {
        associated.RequestDelete();
        return;
    }
    
    associated.angleDeg -= (M_PI / 10.0) * dt;

    // Resting State
    if (state == AlienState::RESTING)
    {
        restTimer.Update(dt);
        if (restTimer.Get() >= 1.5 + timeOffset)
        {
            if (PenguinBody::player == nullptr)
                return;
            
            destination = Camera::pos + Vec2(GameData::WIDTH / 2.0, GameData::HEIGHT / 2.0); // Big brain time
            Vec2 alienPos = associated.box.GetCenter();
            float angle = alienPos.GetAngle(destination) - (M_PI / 4.0);
            speed = Vec2(400, 400).GetRotated(angle);
            state = AlienState::MOVING;
        }
    }

    // Moving State
    else
    {
        // Calculates distance between alienPos <-> destination
        Vec2 alienPos = associated.box.GetCenter();
        float dist = alienPos.GetDistance(destination);

        // Moving to destination
        Vec2 deltaS = (speed * dt);
        if (dist >= deltaS.GetMagnitude())
            associated.box.SetVec(associated.box.GetVec() + deltaS);

        // Finish moving and start blasting
        else
        {
            if (PenguinBody::player == nullptr)
                return;
            
            speed = Vec2(0, 0);
            destination = Camera::pos + Vec2(GameData::WIDTH / 2.0, GameData::HEIGHT / 2.0); // Big brain time
            if (minionArray.size() > 0)
            {
                // Get minion closest to pos
                int idx = 0;
                float shortestDist = 1000000;
                for (unsigned int i = 0; i < minionArray.size(); i++)
                {
                    if (!minionArray[i].expired())
                    {
                        float dist = minionArray[i].lock()->box.GetCenter().GetDistance(destination);
                        shortestDist = min(shortestDist, dist);
                        if (shortestDist == dist)
                            idx = i;
                    }
                }
                if (!minionArray[idx].expired())
                {
                    auto minion = (Minion*) minionArray[idx].lock()->GetComponent("Minion");
                    auto target = destination;
                    minion->Shoot(target);
                }
            }
            state = AlienState::RESTING;
            restTimer.Restart();
        }
    }
}

void Alien::Render()
{

}

bool Alien::Is(const char* type)
{
    string str_type = type;
    return str_type == "Alien";
}