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
#include "../header/DialogBox.h"

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
    AddObject(playerGo, 1);
    AddColliderObject(playerGo);

    // Camera
    Camera::Follow(playerGo);

    // @TODO Tests START here ---
    // Box
    GameObject* testBoxGo = new GameObject();
    TestBox* testBox = new TestBox(*testBoxGo);
    
    testBoxGo->box.SetVec(Vec2(500, 600));
    
    testBoxGo->AddComponent(testBox);
    AddObject(testBoxGo);
    AddColliderObject(testBoxGo);

    // Box 2
    GameObject* testBoxGo2 = new GameObject();
    TestBox* testBox2 = new TestBox(*testBoxGo2);
    
    testBoxGo2->box.SetVec(Vec2(800, 600));
    
    testBoxGo2->AddComponent(testBox2);
    AddObject(testBoxGo2);
    AddColliderObject(testBoxGo2);

    // FPS counter
    fpsCounter = new GameObject();
    CameraFollower* textFollower = new CameraFollower(*fpsCounter, fpsCounter->box.GetVec());
    fpsCounter->AddComponent(textFollower);

    const char* fontFile = "./assets/font/call_me_maybe.ttf";
    const char* textStr = "FPS ";
    int fontSize = 16;
    Text::TextStyle style = Text::BLENDED;
    SDL_Color color = {212, 15, 15, 255};
    
    Text* text = new Text(*fpsCounter, fontFile, fontSize, style, textStr, color);
    fpsCounter->AddComponent(text);
    
    AddObject(fpsCounter);
    AddColliderObject(fpsCounter);
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

    // Deletes GOs
    for (uint32_t i = 0; i < objectArray.size(); i++)
        for (uint32_t j = 0; j < objectArray[i].size(); j++)
            if (objectArray[i][j]->IsDead()) 
                objectArray[i].erase(objectArray[i].begin() + j);

    // Checks for colisions
    for (uint32_t i = 0; i < colliderArray.size(); i++)
    {
        for (uint32_t j = i + 1; j < colliderArray.size(); j++)
        {
            Collider* colliderA = (Collider*) colliderArray[i]->GetComponent("Collider");
            Collider* colliderB = (Collider*) colliderArray[j]->GetComponent("Collider");

            if (colliderA == nullptr || colliderB == nullptr)
                continue;

            if (!Collision::IsColliding(colliderA->box, colliderB->box, colliderArray[i]->angleDeg, colliderArray[j]->angleDeg))
                continue;
            
            colliderArray[i]->NotifyCollision(*colliderArray[j]);
            colliderArray[j]->NotifyCollision(*colliderArray[i]);
            Collision::ResolveCollision(*colliderA, *colliderB);
            
            // Update collisions before next frame
            colliderA->ResolveCollisionUpdate(dt);
            colliderB->ResolveCollisionUpdate(dt);
        }
    }

    // Updates FPS counter
    Text* FPS_Text = (Text*) fpsCounter->GetComponent("Text");
    if (FPS_Text != nullptr)
        FPS_Text->SetText(("FPS " + to_string(floor(GameData::currentFPS))).c_str());
}

void StageState::AddColliderObject(GameObject *object)
{
    colliderArray.push_back(object);
}

void StageState::Render()
{
    RenderArray();
}

StageState::~StageState()
{
    objectArray.clear();
    colliderArray.clear();
    cout << "Object Array deleted successfully!" << endl;
}