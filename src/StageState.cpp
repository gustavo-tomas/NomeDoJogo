#include "../header/StageState.h"
#include "../header/Game.h"
#include "../header/Sound.h"
#include "../header/Vec2.h"
#include "../header/TileSet.h"
#include "../header/TileMap.h"
#include "../header/InputManager.h"
#include "../header/Camera.h"
#include "../header/CameraFollower.h"
#include "../header/Collider.h"
#include "../header/Collision.h"
#include "../header/GameData.h"
#include "../header/EndState.h"
#include "../header/Text.h"
#include "../header/Player.h"
#include "../header/TestBox.h"

StageState::StageState() : State()
{
    cout << "\nStageState created successfully!\n" << endl;
}

void StageState::Start()
{
    LoadAssets();
    StartArray();
    started = true;
}

void StageState::Pause()
{

}

void StageState::Resume()
{
    
}

void StageState::LoadAssets()
{
    // Background
    GameObject* bgGo = new GameObject();
    Sprite* bg = new Sprite(*bgGo, "./assets/image/background.png");
    CameraFollower* cf = new CameraFollower(*bgGo);

    bgGo->AddComponent(bg);
    bgGo->AddComponent(cf);
    AddObject(bgGo);

    // Player
    GameObject* playerGo = new GameObject();
    Player* player = new Player(*playerGo);
    
    playerGo->box.SetVec(Vec2(704, 640));
    
    playerGo->AddComponent(player);
    AddObject(playerGo);

    // Camera
    Camera::Follow(playerGo);

    // Box
    GameObject* testBoxGo = new GameObject();
    TestBox* testBox = new TestBox(*testBoxGo);
    
    testBoxGo->box.SetVec(Vec2(500, 600));
    
    testBoxGo->AddComponent(testBox);
    AddObject(testBoxGo);

    // Box 2
    GameObject* testBoxGo2 = new GameObject();
    TestBox* testBox2 = new TestBox(*testBoxGo2);
    
    testBoxGo2->box.SetVec(Vec2(800, 600));
    
    testBoxGo2->AddComponent(testBox2);
    AddObject(testBoxGo2);

    // FPS counter
    GameObject* textGo = new GameObject();
    CameraFollower* textFollower = new CameraFollower(*textGo, textGo->box.GetVec());
    textGo->AddComponent(textFollower);

    const char* fontFile = "./assets/font/call_me_maybe.ttf";
    const char* textStr = "FPS ";
    int fontSize = 16;
    Text::TextStyle style = Text::BLENDED;
    SDL_Color color = {212, 15, 15, 255};
    
    Text* text = new Text(*textGo, fontFile, fontSize, style, textStr, color);
    textGo->AddComponent(text);
    
    AddObject(textGo);
}

void StageState::Update(float dt)
{
    if (QuitRequested() || PopRequested())
        return;

    // Sets quit requested
    if (InputManager::GetInstance().QuitRequested())
        quitRequested = true;

    // Returns to title screen
    if (InputManager::GetInstance().KeyPress(ESCAPE_KEY))
        popRequested = true;

    // Updates the camera
    Camera::Update(dt);

    // Updates GOs
    UpdateArray(dt);

    for (unsigned long i = 0; i < objectArray.size(); i++)
    {
        // Deletes GOs
        if (objectArray[i]->IsDead())
            objectArray.erase(objectArray.begin() + i);

        // Updates FPS counter - Turning into a component might be better 
        Text* FPS_Text = (Text*) objectArray[i]->GetComponent("Text");
        if (FPS_Text != nullptr)
            FPS_Text->SetText(("FPS " + to_string(floor(GameData::currentFPS))).c_str());

        // Checks for colisions
        else
        {
            for (unsigned long j = i + 1; j < objectArray.size(); j++)
            {
                Collider* colliderA = (Collider*) objectArray[i]->GetComponent("Collider");
                Collider* colliderB = (Collider*) objectArray[j]->GetComponent("Collider");
                if (colliderA != nullptr && colliderB != nullptr)
                {
                    if (Collision::IsColliding(colliderA->box, colliderB->box, objectArray[i]->angleDeg, objectArray[j]->angleDeg))
                    {
                        objectArray[i]->NotifyCollision(*objectArray[j]);
                        objectArray[j]->NotifyCollision(*objectArray[i]);
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

void StageState::Render()
{
    RenderArray();
}

StageState::~StageState()
{
    objectArray.clear();
    cout << "Object Array deleted successfully!" << endl;
}