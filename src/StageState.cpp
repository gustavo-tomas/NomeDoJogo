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
#include "../header/Minion.h"
#include "../header/Bullet.h"
#include "../header/NoteSpawner.h"
#include "../header/NoteTrigger.h"

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
    // Background Music
    backgroundMusic = Music("./assets/audio/doom.mp3");
    backgroundMusic.Play();

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
    playerGo->box.SetVec(Vec2(104, 154));
    playerGo->AddComponent(player);
    AddObject(playerGo, 1);
    
    // Camera
    Camera::Unfollow();
    Camera::Reset();

    // @TODO Tests START here ---

    // Guitar UI
    GameObject* guitarGo = new GameObject();
    Sprite* guitarSprite = new Sprite(*guitarGo, "./assets/image/GuitarNeckFinalized.png");
    CameraFollower* guitarCf = new CameraFollower(*guitarGo, Vec2(60, 400));

    guitarGo->AddComponent(guitarCf);
    guitarGo->AddComponent(guitarSprite);
    AddObject(guitarGo);

    // Health UI
    GameObject* uiGo = new GameObject();
    Sprite* uiSprite = new Sprite(*uiGo, "./assets/image/ui.png");
    CameraFollower* uiCf = new CameraFollower(*uiGo, Vec2(0, 0));

    uiSprite->SetScale(1.5, 1.5);
    uiGo->AddComponent(uiCf);
    uiGo->AddComponent(uiSprite);
    AddObject(uiGo);

    // Minion
    for (int i = 0; i < 2; i++)
    {
        GameObject* minionGo = new GameObject();
        Minion* minion = new Minion(*minionGo, Vec2(704, 100 + i * 150));
        
        minionGo->AddComponent(minion);
        AddObject(minionGo);
    }

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

    // NoteSpawner
    GameObject *spawnerGo = new GameObject(); 
    NoteSpawner *spawner = new NoteSpawner(*spawnerGo, "./assets/sheet_music/music1.txt");
    spawnerGo->AddComponent(spawner);
    AddObject(spawnerGo, 1);

    // NoteTriggers
    
    for (int i = 0; i < 4; i++)
    {
        GameObject *noteTriggerGo = new GameObject(); 
        noteTriggerGo->box.SetVec(Vec2(40, GameData::HEIGHT - 20 * (i + 1)));
        NoteTrigger *noteTrigger = new NoteTrigger(*noteTriggerGo);
        noteTriggerGo->AddComponent(noteTrigger);
        AddObject(noteTriggerGo, 1);
    }
}

void StageState::Update(float dt)
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

    // Updates the camera
    Camera::Update(dt);

    // Updates GOs
    UpdateArray(dt);

    // Deletes GOs
    for (uint32_t i = 0; i < objectArray.size(); i++)
        for (uint32_t j = 0; j < objectArray[i].size(); j++)
            if (objectArray[i][j]->IsDead())
                objectArray[i].erase(objectArray[i].begin() + j--);

    // Checks for colisions
    for (uint32_t i = 0; i < colliderArray.size(); i++)
    {
        for (uint32_t j = i + 1; j < colliderArray.size(); j++)
        {
            if(colliderArray[i].expired() || colliderArray[j].expired())
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

    // Updates FPS counter
    Text* FPS_Text = (Text*) fpsCounter->GetComponent("Text");
    if (FPS_Text != nullptr)
        FPS_Text->SetText(("FPS " + to_string(floor(GameData::currentFPS))).c_str());
}

void StageState::AddColliderObject(weak_ptr<GameObject>& object)
{
    colliderArray.emplace_back(object);
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
