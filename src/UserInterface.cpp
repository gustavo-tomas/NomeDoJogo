#include "../header/UserInterface.h"
#include "../header/CameraFollower.h"
#include "../header/Sprite.h"
#include "../header/State.h"
#include "../header/Game.h"

UserInterface::UserInterface(GameObject& associated) : Component(associated)
{
    State& state = Game::GetInstance().GetCurrentState();

    // Heart
    GameObject* heartGo = new GameObject();
    heartGo->box.SetCenter({25, 25});

    CameraFollower* cameraFollower = new CameraFollower(*heartGo, {25, 25});

    Sprite* sprite = new Sprite(*heartGo, "./assets/image/heart/full.png");
    sprite->SetScale(0.5, 0.5);

    heartGo->AddComponent(sprite);
    heartGo->AddComponent(cameraFollower);
    state.AddObject(heartGo, 20021);
    ui["Heart"] = heartGo;

    // Lifebar
    GameObject* lifebarGo = new GameObject();
    CameraFollower* lifeFollower = new CameraFollower(*lifebarGo, heartGo->box.GetCenter() + Vec2(25, -10));

    Sprite* lifeSprite = new Sprite(*lifebarGo, "./assets/image/lifebar/10.png");
    lifeSprite->SetScale(0.5, 0.5);

    lifebarGo->AddComponent(lifeSprite);
    lifebarGo->AddComponent(lifeFollower);
    state.AddObject(lifebarGo, 20021);
    ui["Lifebar"] = lifebarGo;

    // Manabar
    GameObject* manabarGo = new GameObject();
    CameraFollower* manaFollower = new CameraFollower(*manabarGo, heartGo->box.GetCenter() + Vec2(25, 5));

    Sprite* manaSprite = new Sprite(*manabarGo, "./assets/image/manabar/1.png");
    manaSprite->SetScale(0.5, 0.5);

    manabarGo->AddComponent(manaSprite);
    manabarGo->AddComponent(manaFollower);
    state.AddObject(manabarGo, 20021);
    ui["Manabar"] = manabarGo;
}

void UserInterface::Update(float dt)
{

}

void UserInterface::Render()
{

}

bool UserInterface::Is(const char* type)
{
    string str_type = type;
    return str_type == "UserInterface";
}

void UserInterface::UpdateHeart(int value)
{
    
}

// @TODO: DRY the Updates & use weak_ptr(?)
void UserInterface::UpdateLifebar(int value)
{
    auto lifebar = (Sprite *) ui["Lifebar"]->GetComponent("Sprite");
    if (lifebar != nullptr)
    {
        // HP = 1 <-> 10 refactor needed if HP > 100
        string nextSprite = "./assets/image/lifebar/" + to_string(value) + ".png";
        lifebar->ChangeSprite(nextSprite.c_str());
    }
}

void UserInterface::UpdateManabar(int value)
{
    auto manabar = (Sprite *) ui["Manabar"]->GetComponent("Sprite");
    if (manabar != nullptr)
    {
        // MANA = 1 <-> 4 refactor needed if MANA > 40
        string nextSprite = "./assets/image/manabar/" + to_string(value) + ".png";
        manabar->ChangeSprite(nextSprite.c_str());
    }
}
