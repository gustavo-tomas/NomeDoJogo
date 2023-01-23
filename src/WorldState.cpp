#include "../header/WorldState.h"
#include "../header/Camera.h"
#include "../header/CameraFollower.h"
#include "../header/Collider.h"
#include "../header/Collision.h"
#include "../header/Game.h"
#include "../header/InputManager.h"
#include "../header/Minion.h"
#include "../header/Player.h"
#include "../header/Sprite.h"
#include "../header/StageState.h"


WorldState::WorldState() : State()
{
    
}

WorldState::~WorldState()
{
    objectArray.clear();
    cout << "WorldState deleted successfully!\n";
}

void WorldState::Start()
{
    LoadAssets();
    StartArray();
    started = true; 
}

void WorldState::LoadAssets()
{
    // Background
    GameObject* bgGo = new GameObject();
    Sprite* bg = new Sprite(*bgGo, "./assets/image/parallax-mountain-bg.png");
    bg->SetScale(4.0, 4.0);
    CameraFollower* cf = new CameraFollower(*bgGo);

    bgGo->AddComponent(bg);
    bgGo->AddComponent(cf);
    AddObject(bgGo);

    // Player
    playerGo = new GameObject();
    Player* player = new Player(*playerGo);
    playerGo->box.SetVec(Vec2(104, 154));
    playerGo->AddComponent(player);
    AddObject(playerGo);

    // Camera
    Camera::Follow(playerGo);

    // Minion
    for (int i = 0; i < 2; i++)
    {
        GameObject* minionGo = new GameObject();
        Minion* minion = new Minion(*minionGo, Vec2(704, 100 + i * 150));
        
        minionGo->AddComponent(minion);
        AddObject(minionGo);
    }
}

void WorldState::Update(float dt)
{
    if (QuitRequested() || PopRequested())
        return;

    // Sets quit requested
    if (InputManager::GetInstance().QuitRequested())
    {
        quitRequested = true;
        return;
    }

    // Returns to title screen
    if (InputManager::GetInstance().KeyPress(ESCAPE_KEY))
    {
        popRequested = true;
        return;
    }

    // Creates new StageState
    if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON))
        Game::GetInstance().Push(new StageState());

    // Updates the camera
    Camera::Update(dt);

    // Updates GOs
    UpdateArray(dt);

    for (uint32_t i = 0; i < objectArray.size(); i++)
    {
        for (uint32_t j = 0; j < objectArray[i].size(); j++)
        {
            // Deletes GOs
            if (objectArray[i][j]->IsDead())
                objectArray[i].erase(objectArray[i].begin() + j);

            // Checks for colisions @TODO use new array method when merging with dev
            else
            {
                uint32_t iniK = 0;
                uint32_t iniL = 0;
                
                if (j + 1 == objectArray[i].size())
                {
                    iniK = i + 1;
                    iniL = 0;
                } 
                
                else
                {
                    iniK = i;
                    iniL = j + 1;
                }
                
                for (uint32_t k = iniK; k < objectArray.size(); k++)
                {
                    for (uint32_t l = iniL; l < objectArray[k].size(); l++)
                    {
                        Collider* colliderA = (Collider*) objectArray[i][j]->GetComponent("Collider");
                        Collider* colliderB = (Collider*) objectArray[k][l]->GetComponent("Collider");
                        if (colliderA != nullptr && colliderB != nullptr)
                        {
                            if (Collision::IsColliding(colliderA->box, colliderB->box, objectArray[i][j]->angleDeg, objectArray[k][l]->angleDeg))
                            {
                                objectArray[i][j]->NotifyCollision(*objectArray[k][l]);
                                objectArray[k][l]->NotifyCollision(*objectArray[i][j]);
                                Collision::ResolveCollision(*colliderA, *colliderB);
                                
                                // Update collisions before rendering
                                colliderA->ResolveCollisionUpdate(dt);
                                colliderB->ResolveCollisionUpdate(dt);
                            }
                        }
                    }
                }
            }
        }
    }
}

void WorldState::Render()
{
    RenderArray();
}

void WorldState::Pause()
{
    
}

void WorldState::Resume()
{
    Camera::Follow(playerGo);
}
