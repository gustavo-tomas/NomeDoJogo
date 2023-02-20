#include "../header/TreeState.h"
#include "../header/Camera.h"
#include "../header/CameraFollower.h"
#include "../header/Collider.h"
#include "../header/Collision.h"
#include "../header/GameData.h"
#include "../header/InputManager.h"
#include "../header/Player.h"
#include "../header/Sprite.h"

TreeState::TreeState() : State()
{
    cout << "\nTree State\n";
}

TreeState::~TreeState()
{
    objectArray.clear();
    cout << "TreeState deleted successfully!\n";
}

void TreeState::Start()
{
    LoadAssets();
    StartArray();
    backgroundMusic.Play();
    started = true;
}

void TreeState::LoadAssets()
{
    // Music
    backgroundMusic = Music((GameData::audiosPath + "Soundtrack/Tree_Theme.mp3").c_str(), 20);

    // Background
    GameObject* bgGo = new GameObject();
    Sprite* bg = new Sprite(*bgGo, "./assets/image/background.png");
    bg->SetScale(4.0, 4.0);
    CameraFollower* cf = new CameraFollower(*bgGo);

    bgGo->AddComponent(bg);
    bgGo->AddComponent(cf);
    AddObject(bgGo, -GameData::HEIGHT);

    // Tree
    GameObject* treeGo = new GameObject();
    Sprite* tree = new Sprite(*treeGo, "./assets/image/tree.png");
    tree->SetScale(0.15, 0.15);

    treeGo->AddComponent(tree);
    AddObject(treeGo, 5002);

    // Player
    GameObject* playerGo = new GameObject();
    playerGo->box.SetVec({891, 1451});
    Player* playerComp = new Player(*playerGo);

    playerGo->AddComponent(playerComp);
    player = AddObject(playerGo, 10020);

    // Camera
    Camera::Follow(playerGo);

    // Trigger
    healingArea = Rect(treeGo->box.x, treeGo->box.y, treeGo->box.w + 30, treeGo->box.h + 30);
}

void TreeState::Update(float dt)
{
    if (QuitRequested() || PopRequested())
        return;

    // Sets quit requested
    if (InputManager::GetInstance().QuitRequested())
    {
        quitRequested = true;
        return;
    }

    // Returns to World
    if (InputManager::GetInstance().KeyPress(ESCAPE_KEY) || GameData::returnToMenu)
    {
        popRequested = true;
        return;
    }

    // Heals player
    if (!player.expired())
    {
        if (healingArea.Contains(player.lock()->box.GetCenter()))
            ((Player *) player.lock()->GetComponent("Player"))->RestoreHealth();
    }

    // Updates GOs
    UpdateArray(dt);

    // Deletes GOs
    for (uint32_t i = 0; i < objectArray.size(); i++)
        if (objectArray[i]->IsDead())
            objectArray.erase(objectArray.begin() + i--);

    // Checks for colisions
    for (uint32_t i = 0; i < colliderArray.size(); i++)
    {
        for (uint32_t j = i + 1; j < colliderArray.size(); j++)
        {
            if (colliderArray[i].expired() || colliderArray[j].expired())
                continue;
            
            auto weakColliderA = colliderArray[i].lock().get();
            auto weakColliderB = colliderArray[j].lock().get();

            Collider* colliderA = (Collider*) weakColliderA->GetComponent("Collider");
            Collider* colliderB = (Collider*) weakColliderB->GetComponent("Collider");

            if (colliderA == nullptr || colliderB == nullptr)
                continue;

            if (!Collision::IsColliding(colliderA->box, colliderB->box, weakColliderA->angleDeg, weakColliderB->angleDeg))
                continue;
            
            weakColliderA->NotifyCollision(*weakColliderB);
            weakColliderB->NotifyCollision(*weakColliderA);
            Collision::ResolveCollision(*colliderA, *colliderB);
            
            // Update collisions before next frame
            colliderA->ResolveCollisionUpdate(dt);
            colliderB->ResolveCollisionUpdate(dt);
        }
    }

    // Updates the camera
    Camera::Update(dt);

    // Updates camera followers (o fix mais feio da história)
    for (uint32_t i = 0; i < objectArray.size(); i++)
        if ((CameraFollower *) objectArray[i]->GetComponent("CameraFollower") != nullptr)
            objectArray[i]->GetComponent("CameraFollower") ->Update(dt);
}

void TreeState::Render()
{
    if (GameData::returnToMenu) return;
    RenderArray();
}

void TreeState::Pause()
{
    
}

void TreeState::Resume()
{
    
}
