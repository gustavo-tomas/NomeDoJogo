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
#include "../header/Spawner.h"

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
    Sprite* bg = new Sprite(*bgGo, "./assets/image/parallax-mountain-bg.png");
    bg->SetScale(4.0, 4.0);
    CameraFollower* cf = new CameraFollower(*bgGo);

    bgGo->AddComponent(bg);
    bgGo->AddComponent(cf);
    AddObject(bgGo);

    // Player
    GameObject* playerGo = new GameObject();
    Player* player = new Player(*playerGo);
    playerGo->box.SetVec(Vec2(104, 154));
    playerGo->AddComponent(player);
    AddObject(playerGo);

    // Camera
    // Camera::Follow(playerGo);

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

    // Note
    GameObject* noteGo = new GameObject();
    noteGo->box.SetVec(Vec2(904, 400));

    // @TODO rotation test
    Vec2 pos = noteGo->box.GetCenter() - Vec2(10, 0);
    float ang = noteGo->box.GetCenter().GetAngle(pos) - (M_PI / 4.0);

    Bullet* note = new Bullet(*noteGo, ang, 50, 0, 900,
                     "./assets/image/mage-bullet-13x13.png", 5, 0.7, false);
    
    noteGo->AddComponent(note);
    AddObject(noteGo);

    // @TODO Tests END here ---

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

    GameObject *spawnerGo = new GameObject(); 
    Spawner *spawner = new Spawner(*spawnerGo, "./assets/sheet_music/music1.txt");
    spawnerGo->AddComponent(spawner);
    AddObject(spawnerGo, 1);
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

    for (uint32_t i = 0; i < objectArray.size(); i++)
    {
        for (uint32_t j = 0; j < objectArray[i].size(); j++)
        {
            // Deletes GOs
            if (objectArray[i][j]->IsDead())
                objectArray[i].erase(objectArray[i].begin() + j);

            // Checks for colisions
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

    // Updates FPS counter
    Text* FPS_Text = (Text*) fpsCounter->GetComponent("Text");
    if (FPS_Text != nullptr)
        FPS_Text->SetText(("FPS " + to_string(floor(GameData::currentFPS))).c_str());
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